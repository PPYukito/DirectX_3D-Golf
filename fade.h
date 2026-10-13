#pragma once

#ifndef FADE_H
#define FADE_H

#include "SimdMath.h"

void Fade_Initialize();
void Fade_Finalizde();

void Fade_Update(float delta_time);
void Fade_Draw();

void Fade_Start(bool isFadeIn, float fadeTime, const Vector4& color = {1.0f, 1.0f, 1.0f, 1.0f});

bool Fade_IsFade();

#endif