#include "camera_rotation.h"

#include "direct3d.h"

void RotationCamera::Update(float delta_time)
{
	m_angle += m_angleSpeed * delta_time;
}

Matrix RotationCamera::GetViewMatrix() const
{
	Vector3 camPosition{ m_length, m_height, 0.0f };
	Matrix rotation = Matrix::RotationY(m_angle);
	Matrix translation = Matrix::Translation(m_targetPosition);
	camPosition = camPosition.Transform(rotation * translation);

	return Matrix::LookAtLH(camPosition, m_targetPosition, { 0.0f, 1.0f, 0.0f });
}

Matrix RotationCamera::GetProjectionMatrix() const
{
	const float SCREEN_WIDTH = static_cast<float>(Direct3D_GetBackBufferWidth()); // âÊñ ÇÃïù
	const float SCREEN_HEIGHT = static_cast<float>(Direct3D_GetBackBufferHeight()); // âÊñ ÇÃçÇÇ≥

	return Matrix::PerspectiveFovLH(Math::ToRadians(60), SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);
}

void RotationCamera::SetTarget(const Vector3& targetPosition)
{
	m_targetPosition = targetPosition;
}
