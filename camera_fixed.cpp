#include "camera_fixed.h"

#include "direct3d.h"

Matrix FixedCamera::GetViewMatrix() const
{
    return Matrix::LookAtLH(m_position, m_target_position, { 0.0f, 1.0f, 0.0f });
}

Matrix FixedCamera::GetProjectionMatrix() const
{
	const float SCREEN_WIDTH = static_cast<float>(Direct3D_GetBackBufferWidth()); // âÊñ ÇÃïù
	const float SCREEN_HEIGHT = static_cast<float>(Direct3D_GetBackBufferHeight()); // âÊñ ÇÃçÇÇ≥

	return Matrix::PerspectiveFovLH(Math::ToRadians(40), SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);
}
