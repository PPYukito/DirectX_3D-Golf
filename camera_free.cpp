#include "camera_free.h"
#include "direct3d.h"


void FreeCamera::MoveForward(float distance)
{
	Vector3 forward = m_front;
	forward.y = 0.0f;

	m_position += forward.Normalize() * distance;
}

void FreeCamera::MoveBackward(float distance)
{
	MoveForward(-distance);
}

void FreeCamera::MoveLeft(float distance)
{
	Vector3 left = m_front.Cross({ 0.0f, 1.0f, 0.0f });

	m_position += left.Normalize() * distance;
}

void FreeCamera::MoveRight(float distance)
{
	MoveLeft(-distance);
}

void FreeCamera::MoveUp(float distance)
{
	m_position += {0.0f, distance, 0.0f};
}

void FreeCamera::MoveDown(float distance)
{
	m_position -= {0.0f, distance, 0.0f};
}

void FreeCamera::RotateYaw(float angle)
{
	Matrix rotation_y = Matrix::RotationY(angle);
	m_front = m_front.Transform(rotation_y);
}

void FreeCamera::RotatePitch(float angle)
{
	Vector3 left = m_front.Cross({ 0.0f, 1.0f, 0.0f });

	Matrix rotation_x = Matrix::RotationAxis(left, angle);
	m_front = m_front.Transform(rotation_x);
}

Matrix FreeCamera::GetViewMatrix() const
{
	return Matrix::LookToLH(m_position, m_front, { 0.0f, 1.0f, 0.0f });
}

Matrix FreeCamera::GetProjectionMatrix() const
{
	const float SCREEN_WIDTH = static_cast<float>(Direct3D_GetBackBufferWidth()); // âÊñ ÇÃïù
	const float SCREEN_HEIGHT = static_cast<float>(Direct3D_GetBackBufferHeight()); // âÊñ ÇÃçÇÇ≥
	
	return Matrix::PerspectiveFovLH(Math::ToRadians(60), SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);
}
