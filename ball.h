#pragma once
#include "model.h"
#include "SimdMath.h"
#include "aabb.h"

class Ball
{
private:
	MODEL* m_pModel{ nullptr };
	Vector3 m_position{};
	Vector3 m_rotation{0, 0, 0};
	Vector3 m_velocity{};
	bool canFall = true;
	static constexpr float MASS = 0.046f; // Kg
	const float EPSILON = 0.05f;//1e-5f;

	AABB m_shadow_aabb{};
	Vector3 m_shadow_position{};

public:
	Ball(const Vector3& position);
	~Ball();

	void Update(float delta_time);
	void Draw() const;

	AABB GetAABB() const;

	void OnHit(const AABB::Hit& hit);
	void AddImpulse(const Vector3& impulse);

	bool IsStopped() const;
	const Vector3& GetPosition() const { return m_position; }

	AABB GetShadowAABB() const;
	void OnShadowOverlap(const AABB& aabb);
};
