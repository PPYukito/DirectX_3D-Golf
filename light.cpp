#include "light.h"
#include "constant_buffer.h"


struct Light
{
	Vector4 direction;
	Vector4 color;
	Vector4 ambient; // ŠÂ‹«Œõ
};

static ConstantBuffer<Light>* g_pPSConstantBuffer = nullptr;
static Light g_Light{};


void Light_Initialize()
{
	g_pPSConstantBuffer = new ConstantBuffer<Light>(ShaderType::PixelShader, 0);
}

void Light_Finalize()
{
	delete g_pPSConstantBuffer;
}

void Light_SetDirectional(const Vector3& direction, const Vector3& color)
{
	g_Light.direction = { direction };
	g_Light.color = { color };
}

void Light_SetAmbient(const Vector3& color)
{
	g_Light.ambient = { color };
}

void Light_Update()
{
	g_pPSConstantBuffer->Update(g_Light);
}
