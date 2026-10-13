#pragma once
#include "SimdMath.h"


class AABB
{
private:
	Vector3 m_min{};
	Vector3 m_max{};
	Vector3 m_position{};
	Vector3 m_rotation{};

public:
	AABB() = default;
	AABB(const Vector3& min, const Vector3& max, const Vector3& position, const Vector3& rotation)
		//: m_min(min), m_max(max), m_position(position), m_rotation(rotation)
	{
		m_min = min;// * rotation;
		m_max = max;// * rotation;
		m_position = position;
		m_rotation = rotation;
	}

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	void SetMin(const Vector3& min) { m_min = min; }
	void SetMax(const Vector3& max) { m_max = max; }
	const Vector3& GetMin() const { return m_min; }
	const Vector3& GetMax() const { return m_max; }

	void SetPosition(const Vector3& position) {
		m_position = position;
	}

	void SetRotation(const Vector3& rotation) {
		m_rotation = rotation;
	}

	bool IsOverlap(const AABB& aabb) const;

	struct Hit
	{
		bool IsHit;
		Vector3 Normal;
		float Depth;
	};

	Hit IsHit(const AABB& aabb) const;
};