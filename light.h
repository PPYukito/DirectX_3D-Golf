#pragma once

#include "SimdMath.h"


// 定数バッファなどの作成
void Light_Initialize();

// 定数バッファの解放
void Light_Finalize();

void Light_SetDirectional(const Vector3& direction, const Vector3& color);
void Light_SetAmbient(const Vector3& color);
void Light_Update();