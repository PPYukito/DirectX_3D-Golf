#include "trail.h"

#include "direct3d.h"
#include "texture.h"
#include "camera.h"
#include "sampler_state.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "input_layout.h"
#include "constant_buffer.h"

constexpr int TRAIL_LENGTH{ 30 };

static Vector3 g_Position[TRAIL_LENGTH]; //位置

static ID3D11Buffer* g_VertexBuffer = NULL;
static int g_Texture = -1;

static SamplerState* g_pSamplerState = nullptr;
static VertexShader* g_pVertexShader = nullptr;
static PixelShader* g_pPixelShader = nullptr;
static InputLayout* g_pInputLayout = nullptr;
static ConstantBuffer<Matrix>* g_pVSConstantBuffer0 = nullptr;
static ConstantBuffer<Matrix>* g_pVSConstantBuffer1 = nullptr;
static ConstantBuffer<Matrix>* g_pVSConstantBuffer2 = nullptr;
static ConstantBuffer<Matrix>* g_pVSConstantBuffer3 = nullptr;

struct Vertex
{
	Vector3 position; // 頂点座標
	Vector2 texcoord; // テクスチャ座標
};


void TrailInitialize(void)
{
	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(Vertex) * TRAIL_LENGTH * 2;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		Direct3D_GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);
	}

	// テクスチャ読込
	g_Texture = Texture_Load(L"asset/texture/trail.png");

#pragma region Prepare Shader Stuff
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

	g_pVSConstantBuffer0 = new ConstantBuffer<Matrix>(ShaderType::VertexShader, 0);
	g_pVSConstantBuffer1 = new ConstantBuffer<Matrix>(ShaderType::VertexShader, 1);
	g_pVSConstantBuffer2 = new ConstantBuffer<Matrix>(ShaderType::VertexShader, 2);
	g_pVSConstantBuffer3 = new ConstantBuffer<Matrix>(ShaderType::VertexShader, 3);
#pragma endregion
}

void TrailFinalize(void)
{
	delete g_pSamplerState;
	delete g_pInputLayout;
	delete g_pPixelShader;
	delete g_pVertexShader;

	delete g_pVSConstantBuffer3;
	delete g_pVSConstantBuffer2;
	delete g_pVSConstantBuffer1;
	delete g_pVSConstantBuffer0;
	Texture_Release(g_Texture);

	SAFE_RELEASE(g_VertexBuffer);
}

void TrailUpdate(void)
{

}

void TrailDraw(const Vector3& cameraPosition)
{
	g_pVertexShader->Bind();
	g_pPixelShader->Bind();
	g_pInputLayout->Bind();
	g_pSamplerState->Bind();

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Direct3D_GetDeviceContext()->Map(g_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* vertex = (Vertex*)msr.pData;

	Vector3 cameraPos = cameraPosition;

	//ここで頂点データ入力
	for (int i = 0; i < TRAIL_LENGTH - 1; i++)
	{
		Vector3 cameraDir, trailDir, cross, pos;

		//視線ベクトル
		/*cameraDir.x = g_Position[i].x - cameraPos.x;
		cameraDir.y = g_Position[i].y - cameraPos.y;
		cameraDir.z = g_Position[i].z - cameraPos.z;*/
		cameraDir = g_Position[i] - cameraPos;

		//トレイルの方向ベクトル
		/*trailDir.x = g_Position[i].x - g_Position[i + 1].x;
		trailDir.y = g_Position[i].y - g_Position[i + 1].y;
		trailDir.z = g_Position[i].z - g_Position[i + 1].z;*/
		trailDir = g_Position[i] - g_Position[i + 1];

		//外積
		/*cross.x = cameraDir.y * trailDir.z - cameraDir.z * trailDir.y;
		cross.y = cameraDir.z * trailDir.x - cameraDir.x * trailDir.z;
		cross.z = cameraDir.x * trailDir.y - cameraDir.y * trailDir.x;*/
		cross = cameraDir.Cross(trailDir);

		//ベクトルの長さ
		//float len = sqrtf(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);
		cross.Normalize();

		//正規化（長さを１にする）
		/*if (len != 0.0f)
		{
			cross.x /= len;
			cross.y /= len;
			cross.z /= len;
		}*/


		//上頂点
		/*pos.x = g_Position[i].x + cross.x * 0.2f;
		pos.y = g_Position[i].y + cross.y * 0.2f;
		pos.z = g_Position[i].z + cross.z * 0.2f;*/
		pos = g_Position[i] + cross * 0.2f;

		vertex[i * 2].position = pos;
		//vertex[i * 2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[i * 2].texcoord = { 0.5f, 0.0f };
		//vertex[i * 2].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);


		//下頂点
		/*pos.x = g_Position[i].x - cross.x * 0.2f;
		pos.y = g_Position[i].y - cross.y * 0.2f;
		pos.z = g_Position[i].z - cross.z * 0.2f;*/
		pos = g_Position[i] - cross * 0.2f;

		vertex[i * 2 + 1].position = pos;
		//vertex[i * 2 + 1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[i * 2 + 1].texcoord = { 0.5f, 1.0f };
		//vertex[i * 2 + 1].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	vertex[TRAIL_LENGTH * 2 - 2].position = g_Position[TRAIL_LENGTH - 1];
	//vertex[TRAIL_LENGTH * 2 - 2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[TRAIL_LENGTH * 2 - 2].texcoord = { 0.5f, 0.0f };

	vertex[TRAIL_LENGTH * 2 - 1].position = g_Position[TRAIL_LENGTH - 1];
	//vertex[TRAIL_LENGTH * 2 - 1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[TRAIL_LENGTH * 2 - 1].texcoord = { 0.5f, 1.0f };


	Direct3D_GetDeviceContext()->Unmap(g_VertexBuffer, 0);


	// マトリクス設定
	Matrix projection, view, world, wvp;

	//view = GetCameraView();
	//projection = GetCameraProjection();

	// 頂点バッファ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Direct3D_GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	//ID3D11ShaderResourceView* texture = GetTexture(g_Texture);
	//Direct3D_GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Direct3D_GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ワールドマトリクス
	/*world = XMMatrixIdentity();
	wvp = world * view * projection;
	wvp = XMMatrixTranspose(wvp);
	world = XMMatrixTranspose(world);

	CONSTANT matrix;
	XMStoreFloat4x4(&matrix.wvp, wvp);
	XMStoreFloat4x4(&matrix.world, world);
	matrix.LightEnable = false;*/

	// 頂点シェーダの定数バッファに行列を送信
	//ShaderVertexSetMatrix(&matrix);
	g_pVSConstantBuffer0->Update(Matrix::Identity()); // set world matrix (transposed)

	// if don:t do this, it can bug.
	g_pVSConstantBuffer3->Update(Matrix::Identity()); // set world matrix (transposed)

	Texture_SetTexture(g_Texture);
	// ポリゴン描画
	Direct3D_GetDeviceContext()->Draw(TRAIL_LENGTH * 2, 0);

}


void SetTrailPosition(const Vector3& Position)
{
	for (int i = 0; i < TRAIL_LENGTH - 1; i++)
	{
		g_Position[i] = g_Position[i + 1];
	}

	g_Position[TRAIL_LENGTH - 1] = Position;
}

void Trail_SetViewMatrix(const Matrix& matrix)
{
	// ビュー座標変換行列を作成し、定数バッファへ送る
	g_pVSConstantBuffer1->Update(matrix.Transpose());
}

void Trail_SetProjectitonMatrix(const Matrix& matrix)
{
	g_pVSConstantBuffer2->Update(matrix.Transpose());
}

