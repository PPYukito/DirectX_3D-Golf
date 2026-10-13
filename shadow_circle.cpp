#include "shadow_circle.h"

#include "texture.h"
#include "constant_buffer.h"
#include "sampler_state.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "input_layout.h"
#include "SimdMath.h"

#include <DirectXMath.h>
using namespace DirectX;

static int g_texture_id{ -1 };
static constexpr int NUM_VERTEX = 4; // 頂点数

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ
static SamplerState* g_pSamplerState = nullptr;
static VertexShader* g_pVertexShader = nullptr;
static PixelShader* g_pPixelShader = nullptr;
static InputLayout* g_pInputLayout = nullptr;

static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer0 = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer1 = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer2 = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer3 = nullptr;

// 頂点構造体
struct Vertex
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT2 texcoord; // テクスチャ座標
};

void CircleShadow_Initialize()
{
	g_texture_id = Texture_Load(L"asset/texture/shadow.png");

#pragma region Create Buffer
    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    // 頂点バッファに送り込む頂点データを作成する
    Vertex v[NUM_VERTEX]{
        // Facing Up mesh
        {{-0.5f, 0.0f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, 0.0f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, 0.0f, -0.5f}, {1.0f, 1.0f}},
    };

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = &v;

    Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &g_pVertexBuffer);

    g_pSamplerState = new SamplerState(D3D11_FILTER_ANISOTROPIC,
        D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);

    g_pVertexShader = new VertexShader("asset/shader/shader_billboard_VS.cso");
    g_pPixelShader = new PixelShader("asset/shader/shader_billboard_PS.cso");

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    g_pInputLayout = new InputLayout(
        layout,
        2,
        g_pVertexShader->get_bytecode(),
        g_pVertexShader->get_bytecode_size()
    );

    g_pVSConstantBuffer0 = new ConstantBuffer<XMFLOAT4X4>(ShaderType::VertexShader, 0);
    g_pVSConstantBuffer1 = new ConstantBuffer<XMFLOAT4X4>(ShaderType::VertexShader, 1);
    g_pVSConstantBuffer2 = new ConstantBuffer<XMFLOAT4X4>(ShaderType::VertexShader, 2);
    g_pVSConstantBuffer3 = new ConstantBuffer<XMFLOAT4X4>(ShaderType::VertexShader, 3);
#pragma endregion
}

void CircleShadow_Finalize()
{
    SAFE_RELEASE(g_pVertexBuffer);

    delete g_pVSConstantBuffer3;
    delete g_pVSConstantBuffer2;
    delete g_pVSConstantBuffer1;
    delete g_pVSConstantBuffer0;

    delete g_pInputLayout;
    delete g_pPixelShader;
    delete g_pVertexShader;
    delete g_pSamplerState;

	Texture_Release(g_texture_id);
}

void CircleShadow_SetViewMatrix(const Matrix& matrix)
{
    // ビュー座標変換行列を作成し、定数バッファへ送る
    g_pVSConstantBuffer1->Update(matrix.Transpose());
}

void CircleShadow_SetProjectitonMatrix(const Matrix& matrix)
{
	g_pVSConstantBuffer2->Update(matrix.Transpose());
}

void CircleShadow_Draw(const Vector3& position)
{
    g_pVertexShader->Bind();
    g_pPixelShader->Bind();
    g_pInputLayout->Bind();

    g_pSamplerState->Bind();

    Texture_SetTexture(g_texture_id);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    Direct3D_GetDeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    Matrix matrixPosition = Matrix::Translation(position);
    g_pVSConstantBuffer0->Update(matrixPosition.Transpose()); // set world matrix (transposed)

    // if don:t do this, it can bug.
    // update texture UV 
    g_pVSConstantBuffer3->Update(Matrix::Identity());

    Direct3D_GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    Direct3D_GetDeviceContext()->Draw(NUM_VERTEX, 0);
}
