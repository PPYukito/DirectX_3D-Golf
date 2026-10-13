#include "aabb.h"

bool AABB::IsOverlap(const AABB& aabb) const
{
	Vector3 amin = m_min + m_position;
	Vector3 amax = m_max + m_position;
	Vector3 bmin = aabb.m_min + aabb.m_position;
	Vector3 bmax = aabb.m_max + aabb.m_position;

	return amin.x < bmax.x
		&& amax.x > bmin.x
		&& amin.y < bmax.y
		&& amax.y > bmin.y
		&& amin.z < bmax.z
		&& amax.z > bmin.z;
}

AABB::Hit AABB::IsHit(const AABB& aabb) const
{
	// This means not hit;
	if (!IsOverlap(aabb))
		return {};

	// This means it' hit!
	Vector3 amin = m_min + m_position;
	Vector3 amax = m_max + m_position;
	Vector3 bmin = aabb.m_min + aabb.m_position;
	Vector3 bmax = aabb.m_max + aabb.m_position;

	float depth_x = std::min(amax.x, bmax.x) - std::max(amin.x, bmin.x);
	float depth_y = std::min(amax.y, bmax.y) - std::max(amin.y, bmin.y);
	float depth_z = std::min(amax.z, bmax.z) - std::max(amin.z, bmin.z);

	int shallow_axis = 0;

	if (depth_x < depth_y)
	{
		if (depth_x < depth_z)
		{
			shallow_axis = 0;
		}
		else
		{
			shallow_axis = 2;
		}
	}
	else
	{
		if (depth_y < depth_z)
		{
			shallow_axis = 1;
		}
		else
		{
			shallow_axis = 2;
		}
	}

	Vector3 normal{};
	float depth;

	switch (shallow_axis)
	{
		case 0:
			normal = aabb.m_position.x - m_position.x < 0.0f ? Vector3{ -1.0f, 0.0f, 0.0f } : Vector3{ 1.0f, 0.0f, 0.0f };
			depth = depth_x;
			break;
		case 1:
			normal = aabb.m_position.y - m_position.y < 0.0f ? Vector3{ 0.0f, -1.0f, 0.0f } : Vector3{ 0.0f, 1.0f, 0.0f };
			depth = depth_y;
			break;
		case 2:
			normal = aabb.m_position.z - m_position.z < 0.0f ? Vector3{ 0.0f, 0.0f, -1.0f } : Vector3{ 0.0f, 0.0f, 1.0f };
			depth = depth_z;
			break;
	}

	/*Hit hit;
	hit.IsHit = true;
	hit.Normal = normal;
	hit.Depth = depth;*/

	return {true, normal, depth};
}
