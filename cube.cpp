/*==============================================================================

   3Dキューブ描画 [cube.cpp]
														 Author : Youhei Sato
														 Date   : 2026/04/20
--------------------------------------------------------------------------------

==============================================================================*/
#include "cube.h"

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "direct3d.h"
#include "debug_ostream.h"
#include "texture.h"
#include "sampler_state.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "input_layout.h"
#include "constant_buffer.h"
#include "SimdMath.h"

static constexpr int NUM_VERTEX = 24; // 頂点数
static constexpr int NUM_INDEX = 36;  // インデックス数

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ
static ID3D11Buffer* g_pIndexBuffer = nullptr; // インデックスバッファ

static SamplerState* g_pSamplerState = nullptr;
static VertexShader* g_pVertexShader = nullptr;
static PixelShader* g_pPixelShader = nullptr;
static InputLayout* g_pInputLayout = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer0 = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer1 = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer2 = nullptr;


// 頂点構造体
struct Vertex
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT3 normal;   // 法線
    XMFLOAT4 color;    // 色
    XMFLOAT2 texcoord; // テクスチャ座標
};

void Cube_Initialize()
{
    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    // 頂点バッファに送り込む頂点データを作成する
    Vertex v[NUM_VERTEX]{
        // 手前
        {{-0.5f, 0.5f,-0.5f}, { 0.0f, 0.0f,-1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.25f}},
        {{ 0.5f, 0.5f,-0.5f}, { 0.0f, 0.0f,-1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.25f}},
        {{-0.5f,-0.5f,-0.5f}, { 0.0f, 0.0f,-1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.5f }},
        {{ 0.5f,-0.5f,-0.5f}, { 0.0f, 0.0f,-1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.5f }},
        // 右
        {{ 0.5f, 0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.25f}},
        {{ 0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.25f}},
        {{ 0.5f,-0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.5f }},
        {{ 0.5f,-0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.5f }},
        // 後ろ
        {{ 0.5f, 0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.25f}},
        {{-0.5f, 0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.25f}},
        {{ 0.5f,-0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.5f }},
        {{-0.5f,-0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.5f }},
        // 左
        {{-0.5f, 0.5f, 0.5f}, { 1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.25f}},
        {{-0.5f, 0.5f,-0.5f}, { 1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.25f}},
        {{-0.5f,-0.5f, 0.5f}, { 1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.5f }},
        {{-0.5f,-0.5f,-0.5f}, { 1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.5f }},
        // 上
        {{-0.5f, 0.5f, 0.5f}, { 0.0f, 1.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f }},
        {{ 0.5f, 0.5f, 0.5f}, { 0.0f, 1.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f }},
        {{-0.5f, 0.5f,-0.5f}, { 0.0f, 1.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.25f}},
        {{ 0.5f, 0.5f,-0.5f}, { 0.0f, 1.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.25f}},
        // 下
        {{-0.5f,-0.5f,-0.5f}, { 0.0f,-1.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.75f}},
        {{ 0.5f,-0.5f,-0.5f}, { 0.0f,-1.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.75f}},
        {{-0.5f,-0.5f, 0.5f}, { 0.0f,-1.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f }},
        {{ 0.5f,-0.5f, 0.5f}, { 0.0f,-1.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f }},
    };

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = &v;

    Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &g_pVertexBuffer);

    // インデックスバッファの作成
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned short) * NUM_INDEX;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    // create vertex that can reuse the same position;
    unsigned short index[] { // 結び方の順番データ
         0,  1,  2,  1,  3,  2,
         4,  5,  6,  5,  7,  6,
         8,  9, 10,  9, 11, 10,
        12, 13, 14, 13, 15, 14,
        16, 17, 18, 17, 19, 18,
        20, 21, 22, 21, 23, 22,
    };

    sd.pSysMem = &index;

    Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &g_pIndexBuffer);

    g_pSamplerState = new SamplerState(D3D11_FILTER_ANISOTROPIC,
        D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);

    g_pVertexShader = new VertexShader("asset/shader/shader_vertex_3d.cso");
    g_pPixelShader = new PixelShader("asset/shader/shader_pixel_3d.cso");

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(Vertex, normal),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    g_pInputLayout = new InputLayout(
        layout,
        4,
        g_pVertexShader->get_bytecode(),
        g_pVertexShader->get_bytecode_size()
    );

    g_pVSConstantBuffer0 = new ConstantBuffer<XMFLOAT4X4>(ShaderType::VertexShader, 0);
    g_pVSConstantBuffer1 = new ConstantBuffer<XMFLOAT4X4>(ShaderType::VertexShader, 1);
    g_pVSConstantBuffer2 = new ConstantBuffer<XMFLOAT4X4>(ShaderType::VertexShader, 2);
}

void Cube_Finalize()
{
    delete g_pSamplerState;
    delete g_pInputLayout;
    delete g_pPixelShader;
    delete g_pVertexShader;
    delete g_pVSConstantBuffer2;
    delete g_pVSConstantBuffer1;
    delete g_pVSConstantBuffer0;

    SAFE_RELEASE(g_pIndexBuffer);
    SAFE_RELEASE(g_pVertexBuffer);
}

void Cube_SetViewMatrix(const Matrix& matrix)
{
    // ビュー座標変換行列を作成し、定数バッファへ送る
    g_pVSConstantBuffer1->Update(matrix.Transpose());
}

void Cube_SetProjectitonMatrix(const Matrix& matrix)
{
    g_pVSConstantBuffer2->Update(matrix.Transpose());
}

static Matrix RotationFromEuler(const Vector3& deg)
{
    float rx = Math::ToRadians(deg.x); // pitch (X)
    float ry = Math::ToRadians(deg.y); // yaw   (Y)
    float rz = Math::ToRadians(deg.z); // roll  (Z)

    // Common convention: apply X then Y then Z -> R = Rz * Ry * Rx
    return Matrix::RotationZ(rz) * Matrix::RotationY(ry) * Matrix::RotationX(rx);
}

void Cube_Draw(int texId, const Vector3& position, const Vector3& rotation)
{
	// シェーダー類を描画パイプラインに設定
	g_pVertexShader->Bind();
	g_pPixelShader->Bind();
	g_pInputLayout->Bind();

	// サンプラーを描画パイプラインに設定する
	g_pSamplerState->Bind();

	// 描画パイプラインにテクスチャを設定する
	Texture_SetTexture(texId);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Direct3D_GetDeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // インデックスバッファを描画パイプラインに設定
    Direct3D_GetDeviceContext()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    /* 座標変換 */

	Matrix scale = Matrix::Scaling(1.0f);
    Matrix rotate = RotationFromEuler(rotation);
	Matrix matrixPosition = Matrix::Translation(position);

	Matrix world = scale * rotate * matrixPosition;
	g_pVSConstantBuffer0->Update(world.Transpose()); // set world matrix (transposed)

	// プリミティブトポロジ設定
    Direct3D_GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ポリゴン描画命令発行
    // Direct3D_GetDeviceContext()->Draw(NUM_VERTEX, 0);
    Direct3D_GetDeviceContext()->DrawIndexed(NUM_INDEX, 0, 0);
}
