#include "fade.h"
#include "texture.h"
#include "polygon.h"
#include "direct3d.h"

static float	g_FadeTime			{ 0.0f };
static float	g_ElapsedTime		{ 0.0f };
static int		g_WhiteTextureId	{ -1 };
static bool		g_IsFading			{ false };
static bool		g_IsFadeIn			{ false };
static Vector4	g_Color				{ 1.0f, 1.0f, 1.0f, 1.0f };

void Fade_Initialize()
{
	g_WhiteTextureId = Texture_Load(L"asset/texture/white.png");
}

void Fade_Finalizde()
{
	Texture_Release(g_WhiteTextureId);
}

void Fade_Update(float delta_time)
{
	if (g_IsFading)
	{
		g_ElapsedTime += delta_time;

		if (g_ElapsedTime >= g_FadeTime)
		{
			g_ElapsedTime = g_FadeTime;
			g_IsFading = false;
		}
	}
}

void Fade_Draw()
{
	float alpha = g_IsFadeIn ? g_ElapsedTime / g_FadeTime : 1.0f - g_ElapsedTime / g_FadeTime;
	/*if (alpha < 0)
		alpha = 0;
	if (alpha > 1)
		alpha = 1;*/
	g_Color.w = alpha;

	Polygon_Draw(
		g_WhiteTextureId,
		0.0f,
		0.0f,
		(float)Direct3D_GetBackBufferWidth(),
		(float)Direct3D_GetBackBufferHeight(),
		0.0f,
		0.0f,
		8.0f,
		8.0f,
		g_Color
	);
}

void Fade_Start(bool isFadeIn, float fadeTime, const Vector4& color)
{
	g_IsFadeIn = isFadeIn;
	g_FadeTime = fadeTime;
	g_Color = color;
	g_ElapsedTime = 0.0f;
	
	// Start Fading;
	g_IsFading = true;
}

bool Fade_IsFade()
{
	return g_IsFading;
}
