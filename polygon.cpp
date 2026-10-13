/*==============================================================================

   ポリゴン描画 [polygon.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#include "polygon.h"

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

static constexpr int NUM_VERTEX = 4; // 頂点数

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static SamplerState* g_pSamplerState = nullptr;
static VertexShader* g_pVertexShader = nullptr;
static PixelShader* g_pPixelShader = nullptr;
static InputLayout* g_pInputLayout = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer = nullptr;

// 頂点構造体
struct Vertex
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT4 color;    // 色
    XMFLOAT2 texcoord; // テクスチャ座標
};


void Polygon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // デバイスとデバイスコンテキストのチェック
    if (!pDevice || !pContext) {
        hal::dout << "Polygon_Initialize() : 与えられたデバイスかコンテクストが不正です" << std::endl;
        return;
    }

    // デバイスとデバイスコンテキストの保存
    g_pDevice = pDevice;
    g_pContext = pContext;

    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);

    g_pSamplerState = new SamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR,
        D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_MIRROR);
    g_pVertexShader = new VertexShader("asset/shader/shader_vertex_2d.cso");
    g_pPixelShader = new PixelShader("asset/shader/shader_pixel_2d.cso");

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    g_pInputLayout = new InputLayout(
        layout,
        3,
        g_pVertexShader->get_bytecode(),
        g_pVertexShader->get_bytecode_size()
    );

    g_pVSConstantBuffer = new ConstantBuffer<XMFLOAT4X4>(ShaderType::VertexShader, 0);
}

void Polygon_Finalize(void)
{
    delete g_pSamplerState;
    delete g_pInputLayout;
    delete g_pPixelShader;
    delete g_pVertexShader;
    delete g_pVSConstantBuffer;
    SAFE_RELEASE(g_pVertexBuffer);
}

void Polygon_Draw(int texId, float x, float y, float width, float height,
	float tx, float ty, float twidth, float theight, Vector4 color)
{
	// シェーダー類を描画パイプラインに設定
	g_pVertexShader->Bind();
	g_pPixelShader->Bind();
	g_pInputLayout->Bind();
	// サンプラーを描画パイプラインに設定する
	g_pSamplerState->Bind();

	// 描画パイプラインにテクスチャを設定する
	Texture_SetTexture(texId);
	
	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	// 頂点情報を書き込み
	const float tex_width = static_cast<float>(Texture_Width(texId));
	const float tex_height = static_cast<float>(Texture_Height(texId));
	const float u0 = tx / tex_width;
	const float v0 = ty / tex_height;
	const float u1 = (tx + twidth) / tex_width;
	const float v1 = (ty + theight) / tex_height;

	// 画面の左上から右下に向かう線分を描画する
	v[0].position = { 0.0f + x, 0.0f + y, 0.0f };
	v[0].color = color;
	v[0].texcoord = { u0, v0 };

	v[1].position = { width + x, 0.0f + y, 0.0f };
	v[1].color = color;
	v[1].texcoord = { u1, v0 };

	v[2].position = { 0.0f + x, height + y, 0.0f };
	v[2].color = color;
	v[2].texcoord = { u0, v1 };

	v[3].position = { width + x, height + y, 0.0f };
	v[3].color = color;
	v[3].texcoord = { u1, v1 };

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	/* 頂点シェーダーに変換行列を設定 */
	const float SCREEN_WIDTH = static_cast<float>(Direct3D_GetBackBufferWidth()); // 画面の幅
	const float SCREEN_HEIGHT = static_cast<float>(Direct3D_GetBackBufferHeight()); // 画面の高さ
	XMFLOAT4X4 transpose;
	XMStoreFloat4x4(&transpose, XMMatrixTranspose(
		XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f)));
	g_pVSConstantBuffer->Update(transpose);

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}
