#ifndef TRAIL_H
#define TRAIL_H

#include "SimdMath.h"

void TrailInitialize(void);
void TrailFinalize(void);
void TrailUpdate(void);
void TrailDraw(const Vector3& cameraPosition);

void SetTrailPosition(const Vector3& Position);
void Trail_SetViewMatrix(const Matrix& matrix);
void Trail_SetProjectitonMatrix(const Matrix& matrix);

void SetTrailPosition(const Vector3& Position);

#endif