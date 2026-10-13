#pragma once

/*==============================================================================

   3DÉLÉÖÅ[Éuï`âÊ [cube.h]
														 Author : RatchetBoy
														 Date   : 2026/06/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <d3d11.h>
#include "SimdMath.h"

void BillBoard_Initialize();
void BillBoard_Finalize();
void BillBoard_SetViewMatrix(const Matrix& matrix);
void BillBoard_SetProjectitonMatrix(const Matrix& matrix);
void BillBoard_Draw(int texId, const Vector3& position, const Vector3& camFront, const bool lookAtCamera = true, const float scale = 1.0f);

void BillBoard_Draw(
	int texId,
	const Vector3& position,
	const Vector3& scale,
	const Vector2& texture_position,
	const Vector2& texture_size
	);

#endif // BILLBOARD_H
