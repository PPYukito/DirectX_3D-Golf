#include "object_cube.h"
#include "cube.h"

Matrix CubeObject::ConvertRotate(const Vector3& rotation)
{
	Matrix rot =
		Matrix::RotationZ(Math::ToRadians(rotation.z)) *
		Matrix::RotationY(Math::ToRadians(rotation.y)) *
		Matrix::RotationX(Math::ToRadians(rotation.x));
	return rot;
}

Vector3 CubeObject::Transform(const Vector3& v, const Matrix& m)
{
	Vector3 result;

	result.x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
	result.y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
	result.z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;

	return result;
}

void CubeObject::Draw() const
{
	Cube_Draw(m_texture_id, GetPosition(), GetRotation());
}
