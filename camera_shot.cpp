#include "camera_shot.h"

#include "direct3d.h"
#include "SimdMath.h"

void ShotCamera::RotateYaw(float angle)
{
	Matrix rotation_y = Matrix::RotationY(angle);
	m_front = m_front.Transform(rotation_y);
}

Matrix ShotCamera::GetViewMatrix() const
{
	
	Vector3 camera_position = GetCameraPosition();
	//camera_position.y += 1.0f;
	return Matrix::LookToLH( camera_position, m_front, {0.0f, 1.0f, 0.0f} );
}

Matrix ShotCamera::GetProjectionMatrix() const
{
	const float SCREEN_WIDTH = static_cast<float>(Direct3D_GetBackBufferWidth()); // âÊñ ÇÃïù
	const float SCREEN_HEIGHT = static_cast<float>(Direct3D_GetBackBufferHeight()); // âÊñ ÇÃçÇÇ≥

	return Matrix::PerspectiveFovLH(Math::ToRadians(60), SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);
}
