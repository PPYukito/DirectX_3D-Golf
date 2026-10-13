/*==============================================================================

   ƒ|ƒŠƒSƒ“•`‰æ [polygon.h]
														 Author : Youhei Sato
														 Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef POLYGON_H
#define POLYGON_H

#include <d3d11.h>
#include "SimdMath.h"

void Polygon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Polygon_Finalize(void);
void Polygon_Draw(int texId, float x, float y, float width, float height, 
	float tx, float ty, float twidth, float theight, Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f});

#endif // POLYGON_H
