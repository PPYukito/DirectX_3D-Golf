/*==============================================================================

   アプリケーションクラス [application.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2026/04/15
--------------------------------------------------------------------------------

==============================================================================*/
#include "application.h"
#include "direct3d.h"
#include "texture.h"
#include "polygon.h"
#include "scene.h"
#include "fade.h"

//
//Vector3 GetNormal(float x, float z)
//{
//    float dx = cos(x * 0.1f) * 0.2f;
//    float dz = -sin(z * 0.08f) * 0.096f;
//
//    Vector3 normal(-dx, 1.0f, -dz);
//
//    normal.Normalize();
//
//    return normal;
//}
//

//float Application::GetHeight(float x, float z)
//{
//    float waveX = sin(x * xCurvenessVolume) * xMagnitude;
//    float waveZ = cos(z * zCurvenessVolume) * zMagnitude;
//
//    return waveX + waveZ;
//}

//Matrix Application::GetRotationMatrix(float worldX, float& y, float worldZ)
//{
//    Vector3 normal = GetNormal(worldX, worldZ);
//    Vector3 rightVector = { 1.0, 0.0f, 0.0f };
//
//    Vector3 forward = rightVector.Cross(normal);
//    forward.Normalize();
//
//    Vector3 right = normal.Cross(forward);
//    right.Normalize();
//
//    Matrix rot(
//        right.x, right.y, right.z, 0,
//        normal.x, normal.y, normal.z, 0,
//        forward.x, forward.y, forward.z, 0,
//        0, 0, 0, 1
//    );
//
//    return rot;
//}

Application::Application(HWND hWnd)
{
    Input_Initialize(hWnd); // 入力系の初期化
    Direct3D_Initialize(hWnd); // Direct3Dの初期化
    Texture_Initialize();
    Polygon_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext()); // ポリゴンの初期化
    Fade_Initialize();

    Scene_Initialize();
}

Application::~Application()
{
    Scene_Finalize();

    Fade_Finalizde();
    Polygon_Finalize(); // ポリゴンの終了処理
    Texture_Finalize();
    Direct3D_Finalize(); // Direct3Dの終了処理
    Input_Finalize(); // 入力系の終了
}

void Application::Update(float delta_time)
{
    Input_Update();

    Fade_Update(delta_time);
    Scene_Update(delta_time);
}

void Application::Draw()
{
    Direct3D_Clear(); // バックバッファのクリア

    Scene_Draw();
    Fade_Draw();

    Direct3D_Present();
}
