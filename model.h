#pragma once

#include <unordered_map>

#include <d3d11.h>
#include <DirectXMath.h>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")
#include "SimdMath.h"
#include "aabb.h"


struct MODEL
{
	std::string FileName;  // ロード時のファイル名（キャッシュキーとして使用)
	const aiScene* AiScene = nullptr;

	ID3D11Buffer** VertexBuffer = nullptr;
	ID3D11Buffer** IndexBuffer = nullptr;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> Texture;

	AABB Aabb;
};

void Model_Initialize();
void Model_Finalize();

MODEL* ModelLoad(const char* FileName, float scale = 1.0f, bool bBlender = false);
void ModelRelease(MODEL* model);

void ModelDraw(MODEL* model, const Vector3& position, const Vector3& rotation);
void Model_SetViewMatrix(const Matrix& viewMatrix);
void Model_SetPerspectiveMatrix(const Matrix& perspectiveMatrix);