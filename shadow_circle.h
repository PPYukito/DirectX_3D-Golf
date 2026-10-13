#pragma once

#pragma once

/*==============================================================================

   3DÉLÉÖÅ[Éuï`âÊ [cube.h]
														 Author : RatchetBoy
														 Date   : 2026/06/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SHADOW_CIRCLE_H
#define SHADOW_CIRCLE_H

#include <d3d11.h>
#include "SimdMath.h"

void CircleShadow_Initialize();
void CircleShadow_Finalize();
void CircleShadow_SetViewMatrix(const Matrix& matrix);
void CircleShadow_SetProjectitonMatrix(const Matrix& matrix);
void CircleShadow_Draw(const Vector3& position /*, const Vector3& rotation*/);

//void CircleShadow_Draw(
//	const Vector3& position,
//	const Vector3& scale,
//	const Vector2& texture_position,
//	const Vector2& texture_size
//);

#endif // BILLBOARD_H
