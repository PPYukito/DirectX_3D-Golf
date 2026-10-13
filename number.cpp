#include "number.h"
#include "texture.h"
#include "polygon.h"

static int g_Texture_id{ -1 };
static int g_Digit{ 0 };
static bool g_IsStartWithZero{ false };

static constexpr float NUMBER_FONT_SIZE{ 48 };

static void DrawNumber(int number, float x, float y)
{
	if (number < 0 || number > 9)
		return;

	Polygon_Draw(
		g_Texture_id,
		x,
		y,
		NUMBER_FONT_SIZE,
		NUMBER_FONT_SIZE,
		NUMBER_FONT_SIZE * number,
		0.0f,
		NUMBER_FONT_SIZE,
		NUMBER_FONT_SIZE);
}

void Number_Initialize(int digit, bool startWithZero)
{
	g_Texture_id = Texture_Load(L"asset/texture/number.png");
	g_Digit = digit;
	g_IsStartWithZero = startWithZero;
}

void Number_Finalize()
{
	Texture_Release(g_Texture_id);
}

void Number_Draw(float x, float y, int value)
{
	// Draw from right to left, so the first number is the first digid in the right;
	for (int i = 0; i < g_Digit; i++)
	{ 
		if (!g_IsStartWithZero && value == 0)
			break; // Do not draw number 0

		int n = value % 10;
		value /= 10;
		float dx = x + NUMBER_FONT_SIZE * /*i*/(g_Digit - 1 - i);
		DrawNumber(n, dx, y);
	}
}
