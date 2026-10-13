/*==============================================================================

   3DÉLÉÖÅ[Éuï`âÊ [cube.h]
														 Author : Youhei Sato
														 Date   : 2026/04/20
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef CUBE_H
#define CUBE_H

#include <d3d11.h>
#include "SimdMath.h"

void Cube_Initialize();
void Cube_Finalize();
void Cube_SetViewMatrix(const Matrix& matrix);
void Cube_SetProjectitonMatrix(const Matrix& matrix);
void Cube_Draw(int texId, const Vector3& position, const Vector3& rotation);

#endif // CUBE_H
