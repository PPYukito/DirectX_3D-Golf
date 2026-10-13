#pragma once

#include "SimdMath.h"
#include "aabb.h"

class Object
{
private:
	Vector3 m_position{};
	Vector3 m_rotation{};

public:
	Object() = default;
	Object(const Vector3& position, const Vector3& rotation) : m_position(position), m_rotation(rotation) {}
	virtual ~Object() = default;

	virtual AABB GetAABB() const {
		return {};
	}

	virtual void Draw() const = 0; // èÉêàâºëzä÷êî

protected:
	const Vector3& GetPosition() const { return m_position; }
	const Vector3& GetRotation() const { return m_rotation; }
};

