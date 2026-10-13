#include "scene_game.h"

#include "scene_title.h"
#include "direct3d.h"
#include "cube.h"
#include "billboard.h"
#include "model.h"
#include <stdlib.h>
#include "light.h"

#include "number.h"
#include "fade.h"
#include "texture.h"
#include "shadow_circle.h"
#include "random.h"
#include "trail.h"
#include "polygon.h"

SceneGame::SceneGame()
{
    
}

SceneGame::~SceneGame()
{
    //delete m_pFlag;
    delete m_pBillAnim;
    delete m_pRotationcamera_Hole;
    delete m_pRotationcamera_Course;
    delete m_pShotCamera;
    //delete m_pCurrentCamera;
    delete m_pFreeCamera;
    delete m_pCourse;
    delete m_pBall;
    delete m_depth_stencil_readOnly;
    delete m_depth_stencil_none;
    delete m_depth_stencil_default; // デプスステンシルステートの解放

    delete m_alpha_blend_add;
    delete m_alpha_blend; // アルファブレンドステートの解放

    Texture_Release(m_flag_texture_id);
    Texture_Release(m_testTexture_id);
    Texture_Release(m_coinTexture_id);
    Texture_Release(m_explo_id);

    Number_Finalize();
    BillBoard_Finalize();
    Cube_Finalize();
    Model_Finalize();
    Light_Finalize();
    CircleShadow_Finalize();
    TrailFinalize();
}

float SceneGame::GetHeight(float x, float z)
{
    float height = sin(x * 0.08f + z * xCurvenessVolume) +
        cos(z * 0.15f + x * zCurvenessVolume) * 0.08f;
    return height;
}

Vector3 SceneGame::GetNormal(float x, float z)
{
    float dx = cos(x * xCurvenessVolume) * (xMagnitude * xCurvenessVolume);
    float dz = sin(z * zCurvenessVolume) * (zMagnitude * zCurvenessVolume);

    Vector3 normal(-dx, 1.0f, dz);
    normal.Normalize();

    return normal;
}

Vector3 SceneGame::GetTerrainRotation(float x, float& y, float z)
{
    float spacing = 0.5f;

    y = GetHeight(x, z);

    float hL = GetHeight(x - spacing, z);
    float hR = GetHeight(x + spacing, z);

    float hD = GetHeight(x, z - spacing);
    float hU = GetHeight(x, z + spacing);

    float pitch = atan2(hD - hU, spacing * 2.0f);
    float roll = atan2(hL - hR, spacing * 2.0f);

    return Vector3(
        Math::ToDegrees(pitch),
        0.0f,
        -Math::ToDegrees(roll)
    );
}

void SceneGame::CameraControlCheck(float delta_time)
{
    // Transform;
    if (Input_IsKeyPress(KK_D)) {
        m_pShotCamera->RotateYaw(delta_time * 1.5f);
    }
    if (Input_IsKeyPress(KK_A)) {
        m_pShotCamera->RotateYaw(delta_time * -1.5f);
    }
}

void SceneGame::HitCommandCheck(float delta_time)
{
    // Hit ball key
    if (Input_IsKeyPress(KK_F))
    {
        m_StrokePow += 0.5f * delta_time;
        m_StrokePow = std::min(maxHitPow, m_StrokePow);
    }
    else if (Input_IsKeyRelease(KK_F))
    {
        Vector3 impulse = m_pShotCamera->GetFront() * m_StrokePow;
        impulse.y = 0.5f * (m_StrokePow / maxHitPow);
        m_pBall->AddImpulse(impulse);

        m_StrokeCount++;
        m_StrokePow = 0;
        m_state = State::kFlying;
    }
}

void SceneGame::Initialize()
{
    TrailInitialize();
    CircleShadow_Initialize();
    Model_Initialize();
    Cube_Initialize();
    BillBoard_Initialize();
    Number_Initialize(scoreNumberDigit, true);

    Light_Initialize();
    Light_SetDirectional(Vector3{ 0.0f, -1.0f, 0.0f }.Normalize(), { 0.8f, 0.7f, 0.6f });
    Light_SetAmbient({ 0.2f, 0.0f, 0.3f });
    Light_Update();

    m_alpha_blend = new AlphaBlendState(AlphaBlendState::BlendMode::Alpha); // アルファブレンドステートの生成
    m_alpha_blend_add = new AlphaBlendState(AlphaBlendState::BlendMode::Additive); // アルファブレンドステートの生成

    m_depth_stencil_default = new DepthStencilState(DepthStencilState::DepthStencilMode::Default); // デプスステンシルステートの生成
    m_depth_stencil_none = new DepthStencilState(DepthStencilState::DepthStencilMode::None);
    m_depth_stencil_readOnly = new DepthStencilState(DepthStencilState::DepthStencilMode::ReadOnly);

    m_pFreeCamera = new FreeCamera({ 0.0f, 3.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });

    // CCTV
    /*m_pFixedCamera = new FixedCamera(
        { -2.0f, 3.0f, 40.0f },
        { 0.0f, 4.0f, 8.0f },
        AABB{ { -6.0f, -10.0f, -1.5f }, { 10.0f, 20.0f, 1.5f }, { 0.0f, 10.0f, 40.0f} , {0.0f, 0.0f, 0.0f} }
    );*/

    m_pBall = new Ball({ 0.0f, 2.5f, 8.0f });

    m_pFixedCameras[0] = FixedCamera(
        { -2.0f, 3.0f, 10.0f },
        m_pBall->GetPosition(),
        AABB{ { -6.0f, -10.0f, -1.5f }, { 10.0f, 20.0f, 1.5f }, { 0.0f, 0.0f, 10.0f} , {0.0f, 0.0f, 0.0f} }
    );

    m_pFixedCameras[1] = FixedCamera(
        { 2.0f, 3.0f, 30.0f },
        m_pBall->GetPosition(),
        AABB{ { -10.0f, -10.0f, -1.5f }, { 6.0f, 20.0f, 1.5f }, { 0.0f, 0.0f, 30.0f} , {0.0f, 0.0f, 0.0f} }
    );

    m_pFixedCameras[2] = FixedCamera(
        { 0.5f, 3.0f, 77.0f },
        m_pBall->GetPosition(),
        AABB{ { -8.5f, -10.0f, -10.0f }, { 7.5f, 20.0f, 1.5f }, { 0.0f, 0.0f, 77.0f} , {0.0f, 0.0f, 0.0f} }
    );
    //--------------------------

    m_pShotCamera = new ShotCamera({ 0.0f, 1.2f, 8.0f }, { 2.0f, 1.0f, 75.0f });

    m_coinTexture_id = Texture_Load(L"asset/texture/coin.png");
    m_testTexture_id = Texture_Load(L"asset/texture/knight_anim.png");
    m_explo_id = Texture_Load(L"asset/texture/explosion.png");
    m_flag_texture_id = Texture_Load(L"asset/texture/flag.png");

    m_pBillAnim = new BillboardAnimation(m_explo_id, 9, 3, 341, 341, 0.1f, false);

    m_pRotationcamera_Course = new RotationCamera{ {0.0f, 2.0f, 40.0f}, 10.0f, 20.0f, 0.3f };
    m_pRotationcamera_Hole = new RotationCamera{ {1.0f, 2.0f, 79.0f}, 4.5f, 2.0f, 0.05f };

    m_pCurrentCamera = m_pShotCamera;
    m_pCourse = new Course;
    m_pCourse->Initialize();

    // Create field course
#pragma region Create Course
    for (float z = courseDepth.x; z < courseDepth.y; z++) {
        for (float x = courseWidth.x; x < courseWidth.y; x++)
        {
            bool spawnObstacle = true;

            float px = x + sin(z * 0.2f);
            float py = GetHeight(px, z);
            //Vector3 rotation = GetTerrainRotation(px, py, z);
            Vector3 position = Vector3{ px, py, z };

            if ((z == courseDepth.y - 4 && (px > 0.0f && px < 1.0f)))
            {
                cupPos = position;

                // golf course cup
                position.y -= 0.75f;
                cup.SetPosition(position);

                spawnObstacle = false;
            }

            m_pCourse->Create(
                Course::Kind::kNormal,
                position,
                /*SceneGame::testing ?*/ Vector3{ 0.0f, 0.0f, 0.0f }// : rotation
            );

            // Create trees, Pingpong versions
#pragma region Trees
            if (
                (x >= courseWidth.x && x < courseWidth.x + 3) ||
                (x > courseWidth.y - 3 && x <= courseWidth.y) ||
                (z >= courseDepth.x && z < courseDepth.x + 3) ||
                (z > courseDepth.y - 3 && z <= courseDepth.y)
                )
            {
                m_pCourse->Create(Course::Kind::kTree, { px, py + 0.5f, z }, { 0.0f, GetRandomFloat(0.0f, 180.0f), 0.0f });
            }
#pragma endregion

#pragma region Obstacles
            // Create obstacles
            if (spawnObstacle == false)
            {
                // Spawn Flag
                m_pCourse->Create(
                    Course::Kind::kMax,
                    { px, py + 0.5f, z},
                    { 0.0f, GetRandomFloat(0.0f, 180.0f), 0.0f },
                    true
                );

                continue;
            }

            if (
                x >= courseWidth.x + 3 &&
                x <= courseWidth.y - 3 &&
                z >= courseDepth.x + 3 &&
                z <= courseDepth.y - 3
                )
            {
                float chance = GetRandomFloat(0, 1);
                if (chance < spawnObstacleChance)
                {
                    int num = GetRandomInteger(2, (static_cast<int>(Course::Kind::kMax) - 1));
                    Course::Kind objectKind = static_cast<Course::Kind>(num);

                    m_pCourse->Create(
                        objectKind,
                        {
                            GetRandomFloat(px - spawnObstacleOffset,px + spawnObstacleOffset),
                            py + 0.5f,
                            GetRandomFloat(z - spawnObstacleOffset, z + spawnObstacleOffset)
                        },
                        { 0.0f, GetRandomFloat(0.0f, 180.0f), 0.0f }
                    );
                }
            }
#pragma endregion
        }
#pragma endregion
    }

    Fade_Start(false, 1.0f);
}

void SceneGame::Finalize()
{

}

void SceneGame::Update(float delta_time)
{
    m_pBillAnim->Update(delta_time);

    if (Fade_IsFade())
    {
        m_pRotationcamera_Course->Update(delta_time);

        if (m_pCurrentCamera != m_pRotationcamera_Course)
            m_pCurrentCamera = m_pRotationcamera_Course;

        return;
    }

    m_pBall->Update(delta_time);
    m_pCourse->HitCheck(m_pBall);
    SetTrailPosition(m_pBall->GetPosition());

    switch (m_state)
    {
        case State::kCourseView:

            m_pRotationcamera_Course->Update(delta_time);

            if (m_pCurrentCamera != m_pRotationcamera_Course)
                m_pCurrentCamera = m_pRotationcamera_Course;

            if (Input_IsKeyTrigger(KK_SPACE))
            {
                m_pCurrentCamera = m_pShotCamera;
                m_state = State::kAddressing;
            }

            break;

        case State::kAddressing:

            CameraControlCheck(delta_time);
            HitCommandCheck(delta_time);

            break;

        case State::kFlying:

            // Update FixedCamera
            /*if (m_pFixedCamera->IsOverlap(m_pBall->GetAABB()))
            {
                m_pFixedCamera->SetTargetPoisition(m_pBall->GetPosition());

                if (m_pCurrentCamera != m_pFixedCamera)
                    m_pCurrentCamera = m_pFixedCamera;
            }*/

            for (int i = 0; i < CameraCount; i++)
            {
                if (m_pFixedCameras[i].IsOverlap(m_pBall->GetAABB()))
                {
                    m_pFixedCameras[i].SetTargetPoisition(m_pBall->GetPosition());

                    if (m_pCurrentCamera != &m_pFixedCameras[i])
                        m_pCurrentCamera = &m_pFixedCameras[i];
                }
            }

            if (cup.IsHit(m_pBall->GetAABB()).IsHit)
            {
                Vector3 cameraPosition = m_pBall->GetPosition() + Vector3{ 0.0f, 3.0f, 2.0f };
                *m_pFreeCamera = { cameraPosition, m_pBall->GetPosition() };

                m_state = State::kInHole;
            }
            else if (m_pBall->IsStopped())
            {
                m_pShotCamera->SetBallPoisition(m_pBall->GetPosition() + Vector3(0.0f, 0.5f, 0.0f));
                m_pCurrentCamera = m_pShotCamera;

                m_state = State::kAddressing;
            }
            break;

        case State::kInHole:

            m_pRotationcamera_Hole->SetTarget(m_pBall->GetPosition());
            m_pRotationcamera_Hole->Update(delta_time);

            if (m_pCurrentCamera != m_pRotationcamera_Hole)
                m_pCurrentCamera = m_pRotationcamera_Hole;

            if (!m_SceneChanging)
            {
                if (Input_IsKeyTrigger(KK_U))
                {
                    Fade_Start(true, 1.0f);
                    m_SceneChanging = true;
                }
            }
            else
            {
                if (Fade_IsFade)
                    Scene_Change(new SceneTitle);
            }

            break;
    }
}

void SceneGame::Draw() const
{
    // Funny thing is if you call this draw number before course or ball
    // , it will have blue background behind the number;

    m_alpha_blend->Bind(); // アルファブレンドステートのバインド

    m_depth_stencil_default->Bind(); // デプスステンシルステートのバインド

#pragma region Update View and Projection Matrix
    Cube_SetViewMatrix(m_pCurrentCamera->GetViewMatrix());
    Cube_SetProjectitonMatrix(m_pCurrentCamera->GetProjectionMatrix());

    // Set billboard's matrix;
    BillBoard_SetViewMatrix(m_pCurrentCamera->GetViewMatrix());
    BillBoard_SetProjectitonMatrix(m_pCurrentCamera->GetProjectionMatrix());

    CircleShadow_SetViewMatrix(m_pCurrentCamera->GetViewMatrix());
    CircleShadow_SetProjectitonMatrix(m_pCurrentCamera->GetProjectionMatrix());

    Trail_SetViewMatrix(m_pCurrentCamera->GetViewMatrix());
    Trail_SetProjectitonMatrix(m_pCurrentCamera->GetProjectionMatrix());
#pragma endregion

    m_pCourse->Draw();
    //CircleShadow_Draw(m_pBall->GetPosition() - Vector3{0.0f, 0.7f, 0.0f});
    m_pBall->Draw();

    //Draw flag
    BillBoard_Draw(m_flag_texture_id, cupPos + Vector3{0.0f, 2.2f, 0.0f}, m_pShotCamera->GetFront(), false, 7.0f);

    TrailDraw(m_pCurrentCamera->GetCameraPosition());

    //------------------------------------------------------------------
    //m_depth_stencil_readOnly->Bind();
    // this will overwrite the previous polygon
    // make come after polygon draw over the previous one? <- I think...

    // make like a clear crystal style effect
    //m_alpha_blend_add->Bind();
    //---------------------------------------
    
    /*BillBoard_Draw(
        m_testTexture_id,
        { 0.0f, 2.0f,  8.0f },
        { 1, 1, 1 },
        m_anim.GetTextureCutPosition(),
        m_anim.GetTexureCutSize()
    );*/

    if (m_state == SceneGame::State::kInHole)
        m_pBillAnim->Draw(m_pBall->GetPosition() , { 0.5f, 0.5f, 0.5f });

    /*BillBoard_Draw(m_testTexture_id, { 0.0f, 2.0f,  8.0f });
    BillBoard_Draw(m_testTexture_id, { 0.0f, 2.0f,  9.0f });
    BillBoard_Draw(m_testTexture_id, { 0.0f, 2.0f, 10.0f });
    BillBoard_Draw(m_testTexture_id, { 0.0f, 2.0f, 11.0f });*/

    m_depth_stencil_none->Bind();

    m_alpha_blend->Bind(); // アルファブレンドステートのバインド
    // Score Draw
    Number_Draw(
        Direct3D_GetBackBufferWidth() - 48 * (scoreNumberDigit + 0.5f), // In this case, width is 1024;
        Direct3D_GetBackBufferHeight() - 48 * 2.0f,
        m_StrokeCount);
}
