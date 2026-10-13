#include "direct3d.h"
#include "texture.h"
#include "model.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "WICTextureLoader11.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "sampler_state.h"
#include "input_layout.h"
#include "constant_buffer.h"
#include "SimdMath.h"
#include <vector>
#include <algorithm>
#include <unordered_map>

// 3D頂点構造体
struct Vertex3d
{
	XMFLOAT3 position; // 座標
	XMFLOAT3 normal;   // 法線
	XMFLOAT4 color;    // 色
	XMFLOAT2 texcoord; // UV
};

static int g_TextureWhite = -1;

// フライウェイト: ロード済みモデルキャッシュ（ファイルパス -> MODEL*）
static std::unordered_map<std::string, MODEL*> g_LoadedModels;

static SamplerState* g_pSamplerState = nullptr;
static VertexShader* g_pVertexShader = nullptr;
static PixelShader* g_pPixelShader = nullptr;
static InputLayout* g_pInputLayout = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer0 = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer1 = nullptr;
static ConstantBuffer<XMFLOAT4X4>* g_pVSConstantBuffer2 = nullptr;


void Model_Initialize()
{
	// シェーダー初期化
	g_pSamplerState = new SamplerState(D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);

	g_pVertexShader = new VertexShader("asset/shader/shader_vertex_3d.cso");
	g_pPixelShader = new PixelShader("asset/shader/shader_pixel_3d.cso");

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(Vertex3d, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(Vertex3d, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex3d, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, offsetof(Vertex3d, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	// ホワイトテクスチャロード
	g_TextureWhite = Texture_Load(L"asset/texture/white.png");
}

void Model_Finalize()
{
	// ロード済みモデルキャッシュが残っていたら破棄
	// イテレータ無効化を避けるため、先にキーをコピー
	std::vector<std::string> keys;
	for (const auto& pair : g_LoadedModels)
	{
		keys.push_back(pair.first);
	}

	for (const auto& key : keys)
	{
		auto it = g_LoadedModels.find(key);
		if (it != g_LoadedModels.end())
		{
			ModelRelease(it->second);
		}
	}
	g_LoadedModels.clear();

	// シェーダーリソースの破棄
	delete g_pSamplerState;
	delete g_pInputLayout;
	delete g_pPixelShader;
	delete g_pVertexShader;
	delete g_pVSConstantBuffer2;
	delete g_pVSConstantBuffer1;
	delete g_pVSConstantBuffer0;
	g_pSamplerState = nullptr;
	g_pVertexShader = nullptr;
	g_pPixelShader = nullptr;
	g_pInputLayout = nullptr;
	g_pVSConstantBuffer0 = nullptr;
	g_pVSConstantBuffer1 = nullptr;
	g_pVSConstantBuffer2 = nullptr;
}


MODEL* ModelLoad( const char *FileName, float scale, bool bBlender)
{
	// フライウェイト: キャッシュから検索
	auto cacheIt = g_LoadedModels.find(FileName);
	if (cacheIt != g_LoadedModels.end())
	{
		// キャッシュに存在 -> 既にロード済み
		return cacheIt->second;
	}

	// キャッシュに無い -> 新規ロード
	MODEL* model = new MODEL;

	// ファイル名を保存（キャッシュキー）
	model->FileName = FileName;

	model->AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(model->AiScene);

	model->VertexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];
	model->IndexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];

	Vector3 min{};
	Vector3 max{};

	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = model->AiScene->mMeshes[m];

		// 頂点バッファ生成
		{
			Vertex3d* vertex = new Vertex3d[mesh->mNumVertices];

			if (m == 0) {
				min = { mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z };
				max = { mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z };
			}

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				if (bBlender) {
					vertex[v].position = XMFLOAT3(mesh->mVertices[v].x * scale, -mesh->mVertices[v].z * scale, mesh->mVertices[v].y * scale);
					vertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, -mesh->mNormals[v].z, mesh->mNormals[v].y);
				}
				else {
					vertex[v].position = XMFLOAT3(mesh->mVertices[v].x * scale, mesh->mVertices[v].y * scale, mesh->mVertices[v].z * scale);
					vertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				}

				vertex[v].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertex[v].texcoord = XMFLOAT2( mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);

				min.x = std::min(vertex[v].position.x, min.x);
				min.y = std::min(vertex[v].position.y, min.y);
				min.z = std::min(vertex[v].position.z, min.z);
				max.x = std::max(vertex[v].position.x, max.x);
				max.y = std::max(vertex[v].position.y, max.y);
				max.z = std::max(vertex[v].position.z, max.z);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Vertex3d) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &model->VertexBuffer[m]);
	
			delete[] vertex;
		}

		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &model->IndexBuffer[m]);

			delete[] index;
		}
	}

	model->Aabb = { min, max, {}, {} }; // モデル全体を覆うAABB

	// FBXにテクスチャが内包されている場合
	for (unsigned int i = 0; i < model->AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = model->AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;
		ID3D11Resource* resource;

		CreateWICTextureFromMemory(
			Direct3D_GetDevice(),
			Direct3D_GetDeviceContext(),
			(const uint8_t*)aitexture->pcData,
			(size_t)aitexture->mWidth,
			&resource,
			&texture);

		assert(texture);

		resource->Release(); // !!!!!!!!!!

		model->Texture[aitexture->mFilename.data] = texture;
	}

	// fbxのファイルパスだけ取得
	const std::string modelPath(FileName);

	// 最後の '/' または '\\' の位置を探す（Windows対応）
	size_t pos = modelPath.find_last_of("/\\");
	std::string directory;

	if (pos != std::string::npos) {
		directory = modelPath.substr(0, pos);  // ファイル名を除いた部分
	}
	else {
		directory = "";  // パスに区切りがない場合（ファイル名のみ）
	}

	// テクスチャがFBXとは別に用意されている場合
	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		aiString filename;
		aiMaterial* aimaterial = model->AiScene->mMaterials[model->AiScene->mMeshes[m]->mMaterialIndex];
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &filename);

		if (filename.length == 0) {
			continue;
		}

		if (model->Texture.count(filename.C_Str())) {
			continue;
		}

		// テクスチャパスに余計なフォルダ名があったらカット
		std::string str_filename = filename.C_Str();
		size_t n = str_filename.find_last_of("\\/");
		str_filename = str_filename.substr(n + 1);

		if (model->Texture.count(str_filename)) {
			continue;
		}

		ID3D11ShaderResourceView* texture;

		std::string texfilename = directory + "/" + str_filename;

		int len = MultiByteToWideChar(CP_UTF8, 0, texfilename.c_str(), -1, nullptr, 0);
		wchar_t* pWideFilename = new wchar_t[len];
		MultiByteToWideChar(CP_UTF8, 0, texfilename.c_str(), -1, pWideFilename, len);

		// テクスチャをロード
		ID3D11Resource* pResource;
		CreateWICTextureFromFile(
			Direct3D_GetDevice(),
			Direct3D_GetDeviceContext(),
			pWideFilename,
			&pResource,
			&texture);

		assert(texture);
		pResource->Release();

		delete[] pWideFilename;

		model->Texture[filename.C_Str()] = texture;
	}

	// ロード済みモデルリストに追加
	g_LoadedModels[FileName] = model;

	return model;
}


void ModelRelease(MODEL* model)
{
	if (model == nullptr) return;

	// キャッシュキーを保存
	std::string cacheKey = model->FileName;

	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		model->VertexBuffer[m]->Release();
		model->IndexBuffer[m]->Release();
	}

	delete[] model->VertexBuffer;
	delete[] model->IndexBuffer;

	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : model->Texture)
	{
		pair.second->Release();
	}

	aiReleaseImport(model->AiScene);

	delete model;

	// ロード済みモデルキャッシュから削除
	auto it = g_LoadedModels.find(cacheKey);
	if (it != g_LoadedModels.end())
	{
		g_LoadedModels.erase(it);
	}
}

static Matrix RotationFromEuler(const Vector3& deg)
{
	float rx = Math::ToRadians(deg.x); // pitch (X)
	float ry = Math::ToRadians(deg.y); // yaw   (Y)
	float rz = Math::ToRadians(deg.z); // roll  (Z)

	// Common convention: apply X then Y then Z -> R = Rz * Ry * Rx
	return Matrix::RotationZ(rz) * Matrix::RotationY(ry) * Matrix::RotationX(rx);
}

void ModelDraw(MODEL* model, const Vector3& position, const Vector3& rotation)
{
	// シェーダー類を描画パイプラインに設定
	g_pVertexShader->Bind();
	g_pPixelShader->Bind();
	g_pInputLayout->Bind();

	// サンプラーを描画パイプラインに設定する
	g_pSamplerState->Bind();

	// ワールド座標変換行列を定数バッファへ送る
	Matrix scale = Matrix::Scaling(1.0f);
	Matrix rotate = RotationFromEuler(rotation);
	Matrix pos = Matrix::Translation(position);

	Matrix world = scale * rotate * pos;
	g_pVSConstantBuffer0->Update(world.Transpose());

	// プリミティブトポロジ設定
	Direct3D_GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		aiString texture;
		aiMaterial* aimaterial = model->AiScene->mMaterials[model->AiScene->mMeshes[m]->mMaterialIndex];
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);

		if( texture.length != 0 ) {
			Direct3D_GetDeviceContext()->PSSetShaderResources(0, 1, &model->Texture[texture.data]);
		}
		else {
			Texture_SetTexture(g_TextureWhite);
		}

		// 頂点バッファを描画パイプラインに設定
		UINT stride = sizeof(Vertex3d);
		UINT offset = 0;
		Direct3D_GetDeviceContext()->IASetVertexBuffers(0, 1, &model->VertexBuffer[m], &stride, &offset);

		// インデックスバッファを描画パイプラインに設定
		Direct3D_GetDeviceContext()->IASetIndexBuffer(model->IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// ポリゴン描画命令発行
		Direct3D_GetDeviceContext()->DrawIndexed(model->AiScene->mMeshes[m]->mNumFaces * 3, 0, 0);
	}
}

// ビュー行列とプロジェクション行列を設定
void Model_SetViewMatrix(const Matrix& viewMatrix)
{
	g_pVSConstantBuffer1->Update(viewMatrix.Transpose());
}

void Model_SetPerspectiveMatrix(const Matrix& perspectiveMatrix)
{
	g_pVSConstantBuffer2->Update(perspectiveMatrix.Transpose());
}
