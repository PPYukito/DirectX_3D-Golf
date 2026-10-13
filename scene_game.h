#pragma once

#include "scene.h"

#include "input.h"
#include "alpha_blend_state.h"
#include "depth_stencil_state.h"
#include "course.h"
#include "camera_free.h"
#include "camera_fixed.h"
#include "camera_shot.h"
#include "camera_rotation.h"
#include "ball.h"
#include "billboard_animation.h"

class SceneGame : public Scene
{
private:
	AlphaBlendState* m_alpha_blend{};
	AlphaBlendState* m_alpha_blend_add{};

	DepthStencilState* m_depth_stencil_default{};
	DepthStencilState* m_depth_stencil_none{};
	DepthStencilState* m_depth_stencil_readOnly{};

	Course* m_pCourse{ nullptr };

	Camera* m_pCurrentCamera{ nullptr };

	FreeCamera* m_pFreeCamera{ nullptr };

	static constexpr size_t CameraCount = 3;
	FixedCamera* m_pFixedCameras = new FixedCamera[CameraCount];

	ShotCamera* m_pShotCamera{ nullptr };
	RotationCamera* m_pRotationcamera_Course{ nullptr };
	RotationCamera* m_pRotationcamera_Hole{ nullptr };

	Ball* m_pBall{ nullptr };

	float GetHeight(float x, float z);
	Vector3 GetNormal(float x, float z);
	//Matrix GetRotationMatrix(float worldX, float& y, float worldZ);

	void CameraControlCheck(float delta_time);
	void HitCommandCheck(float delta_time);

	Vector3 GetTerrainRotation(float x, float& y, float z);

	const int scoreNumberDigit{ 3 };

	const Vector2 courseWidth{-10, 10 };
	const Vector2 courseDepth{  0, 80 };
	const float spawnObstacleChance{ 0.025f };
	const float spawnObstacleOffset{ 0.38f };

	enum class State
	{
		kCourseView,
		kAddressing,
		kFlying,
		kInHole
	};

	State m_state = State::kCourseView;
	int m_StrokeCount = 0;
	float m_StrokePow{ 0.0f };

	const bool testing = false;

	bool m_SceneChanging{ false };

	const float xCurvenessVolume = 0.01f;
	const float xMagnitude = 2.0f;
	const float zCurvenessVolume = 0.008f;
	const float zMagnitude = 1.0f;
	const float maxHitPow = 0.85f;

	AABB cup{ {-0.5f, -0.5f, -0.5f}, { 0.5f, 0.85f, 0.5f}, { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f} };

	int m_testTexture_id{-1};
	int m_coinTexture_id{-1};
	int m_explo_id{-1};

	BillboardAnimation* m_pBillAnim;
	//BillboardAnimation* m_pFlag;

	int m_flag_texture_id{ -1 };
	Vector3 cupPos{0.0f, 0.0f, 0.0f};

public:
	SceneGame();
	~SceneGame();

	void Initialize() override;
	void Finalize() override;
	void Update(float delta_time) override;
	void Draw() const override;
};