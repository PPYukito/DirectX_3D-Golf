#pragma once

#include "object.h"
#include "SimdMath.h"

class CubeObject : public Object
{
private:
	int m_texture_id{ -1 };
	AABB m_aabb;
	Matrix ConvertRotate(const Vector3& rotation);
	Vector3 Transform(const Vector3& v, const Matrix& m);

public:
	CubeObject() = default;
	CubeObject(const Vector3& position, const Vector3& rotation, int texture_id) : Object(position, rotation)
	{
		m_texture_id = texture_id;

		/*Matrix rot = ConvertRotate(rotation);
		Vector3 rotateMin = Transform({ -0.5f, -0.5f, -0.5f }, rot);
		Vector3 rotateMax = Transform({ 0.5f,  0.5f,  0.5f }, rot);
		m_aabb.SetMin(rotateMin);
		m_aabb.SetMax(rotateMax);*/

		m_aabb.SetMin({ -0.5f, -0.5f, -0.5f });
		m_aabb.SetMax({  0.5f,  0.5f,  0.5f });
		m_aabb.SetPosition(position);
		//m_aabb.SetRotation(rotation);
	}
	~CubeObject() = default;

	AABB GetAABB() const override {
		return m_aabb;
	}

	void Draw() const override;
};