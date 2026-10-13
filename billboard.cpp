/*==============================================================================

   3Dキューブ描画 [cube.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2026/04/20
--------------------------------------------------------------------------------

==============================================================================*/
#include "billboard.h"

#include <algorithm>
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
static Matrix g_MtxViewInverse;


// 頂点構造体
struct Vertex
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT2 texcoord; // テクスチャ座標
};

void BillBoard_Initialize()
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
        {{-0.5f,  0.5f, 0.0f}, { 0.0f, 0.0f }},
        {{ 0.5f,  0.5f, 0.0f}, { 1.0f, 0.0f }},
        {{-0.5f, -0.5f, 0.0f}, { 0.0f, 1.0f }},
        {{ 0.5f, -0.5f, 0.0f}, { 1.0f, 1.0f }},
    };

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = &v;

    Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &g_pVertexBuffer);

    // インデックスバッファの作成
    //bd.Usage = D3D11_USAGE_DEFAULT;
    //bd.ByteWidth = sizeof(unsigned short) * NUM_INDEX;
    //bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    //bd.CPUAccessFlags = 0;

    //unsigned short index[]{ // 結び方の順番データ
    //     0,  1,  2,  1,  3,  2,
    //     4,  5,  6,  5,  7,  6,
    //     8,  9, 10,  9, 11, 10,
    //    12, 13, 14, 13, 15, 14,
    //    16, 17, 18, 17, 19, 18,
    //    20, 21, 22, 21, 23, 22,
    //};

    //sd.pSysMem = &index;

    //Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &g_pIndexBuffer);
    //------------ Create Index Buffer --------------

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
}

void BillBoard_Finalize()
{
    delete g_pSamplerState;
    delete g_pInputLayout;
    delete g_pPixelShader;
    delete g_pVertexShader;
    
    delete g_pVSConstantBuffer3;
    delete g_pVSConstantBuffer2;
    delete g_pVSConstantBuffer1;
    delete g_pVSConstantBuffer0;
    //SAFE_RELEASE(g_pIndexBuffer);
    SAFE_RELEASE(g_pVertexBuffer);
}

void BillBoard_SetViewMatrix(const Matrix& matrix)
{
    g_MtxViewInverse = matrix;
    g_MtxViewInverse._41 = 0;
    g_MtxViewInverse._42 = 0;
    g_MtxViewInverse._43 = 0;
    g_MtxViewInverse = g_MtxViewInverse.Inverse();

    // ビュー座標変換行列を作成し、定数バッファへ送る
    g_pVSConstantBuffer1->Update(matrix.Transpose());
}

void BillBoard_SetProjectitonMatrix(const Matrix& matrix)
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

template<typename T>
T Clamp(T value, T min, T max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void BillBoard_Draw(int texId, const Vector3& position, const Vector3& camFront, const bool lookAtCamera, const float scale)
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
    //Direct3D_GetDeviceContext()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    /* 座標変換 */
    Vector3 inverseCame = camFront;
    inverseCame.y = 0;

    Vector3 forward = { 0.0f, 0.0f, 1.0f };
    float dot =
        inverseCame.x * forward.x +
        inverseCame.y * forward.y +
        inverseCame.z * forward.z;
    float angleRadians = acosf(Clamp(dot, -1.0f, 1.0f));
    float angleDegrees = XMConvertToDegrees(angleRadians);

    Matrix scaling = Matrix::Scaling(scale);
    Matrix rotate = RotationFromEuler({0.0f, angleDegrees, 0.0f});
    Matrix positionMatrix = Matrix::Translation(position);

    Matrix worldMatrix{ scaling * rotate * positionMatrix };

    Matrix worldInverse = lookAtCamera ? (g_MtxViewInverse * worldMatrix) : worldMatrix;
    //Matrix worldInverse = g_MtxViewInverse * translation;

    Matrix billboardMatrix{ worldInverse };
    
    g_pVSConstantBuffer0->Update(billboardMatrix.Transpose()); // set world matrix (transposed)
    
    // if don:t do this, it can bug.
    g_pVSConstantBuffer3->Update(Matrix::Identity()); // set world matrix (transposed)

    // プリミティブトポロジ設定
    Direct3D_GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // ポリゴン描画命令発行
    // Direct3D_GetDeviceContext()->Draw(NUM_VERTEX, 0);
    Direct3D_GetDeviceContext()->Draw(NUM_VERTEX, 0);
}

void BillBoard_Draw(int texId, const Vector3& position, const Vector3& scale, const Vector2& texture_position, const Vector2& texture_size)
{
    float tw = texture_size.x / Texture_Width(texId);
    float th = texture_size.y / Texture_Height(texId);
    float tx = texture_position.x / Texture_Width(texId);
    float ty = texture_position.y / Texture_Height(texId);

    Matrix mtxUV = Matrix::Scaling(tw, th, 1.0f)
        * Matrix::Translation(tx, ty, 0.0f);

    g_pVSConstantBuffer3->Update(mtxUV.Transpose());

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
    //Direct3D_GetDeviceContext()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    /* 座標変換 */
    Matrix scaling = Matrix::Scaling(scale);
    Matrix matrixPosition = scaling * Matrix::Translation(position);

    Matrix billboardMatrix{ g_MtxViewInverse * matrixPosition };

    g_pVSConstantBuffer0->Update(billboardMatrix.Transpose()); // set world matrix (transposed)

    // プリミティブトポロジ設定
    Direct3D_GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // ポリゴン描画命令発行
    // Direct3D_GetDeviceContext()->Draw(NUM_VERTEX, 0);
    Direct3D_GetDeviceContext()->Draw(NUM_VERTEX, 0);
}
