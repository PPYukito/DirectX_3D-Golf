/*==============================================================================

   テクスチャ管理 [texture.cpp]
														 Author : Youhei Sato
														 Date   : 2025/06/13
--------------------------------------------------------------------------------

==============================================================================*/
#include "texture.h"
#include "direct3d.h"
#include <string>
#include "WICTextureLoader11.h"
using namespace DirectX;

#include <map>
#include <unordered_map>

struct Texture
{
	ID3D11ShaderResourceView* pTexture{};
	unsigned int width{};
	unsigned int height{};

	Texture() = default;
	Texture(ID3D11ShaderResourceView* texture, unsigned int w, unsigned int h)
		: pTexture(texture), width(w), height(h) {}

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept
		: pTexture(other.pTexture), width(other.width), height(other.height)
	{
		other.pTexture = nullptr;
		other.width = 0;
		other.height = 0;
	}

	Texture& operator=(Texture&& other) noexcept
	{
		if (this != &other) {
			if (pTexture) {
				pTexture->Release();
			}
			pTexture = other.pTexture;
			width = other.width;
			height = other.height;
			other.pTexture = nullptr;
			other.width = 0;
			other.height = 0;
		}
		return *this;
	}

	~Texture() {
		if (pTexture) {
			pTexture->Release();
			pTexture = nullptr;
		}
	}
};


static std::unordered_map<std::wstring, int> g_TextureMap; // ファイル名からテクスチャIDへのマップ
static std::unordered_map<int, std::wstring> g_TextureIDMap; // テクスチャIDからファイル名への逆引きマップ
static std::map<int, Texture> g_TextureData; // テクスチャIDからテクスチャデータへのマップ
static int g_NextTextureID = 0; // 次に割り当てるテクスチャID

void Texture_Initialize()
{
	Texture_AllRelease();
}

void Texture_Finalize(void)
{
	Texture_AllRelease();
}

int Texture_Load(const wchar_t* pFilename)
{
	// すでに読み込んだファイルは読み込まない
	auto it = g_TextureMap.find(pFilename);
	if (it != g_TextureMap.end()) {
		return it->second;
	}

	// テクスチャの読み込み
	HRESULT hr;

	ID3D11Device* pDevice = Direct3D_GetDevice();
	ID3D11DeviceContext* pContext = Direct3D_GetDeviceContext();
	ID3D11Resource* pResource = nullptr;
	ID3D11ShaderResourceView* pTextureView = nullptr;

	hr = CreateWICTextureFromFile(pDevice, pContext, pFilename, &pResource, &pTextureView);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"テクスチャの読み込みに失敗しました", pFilename, MB_OK | MB_ICONERROR);
		return -1;
	}

	ID3D11Texture2D* pTexture = nullptr;
	hr = pResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pTexture));
	if (FAILED(hr) || pTexture == nullptr) {
		if (pTextureView) {
			pTextureView->Release();
		}
		if (pResource) {
			pResource->Release();
		}
		MessageBoxW(nullptr, L"テクスチャリソース型が不正です", pFilename, MB_OK | MB_ICONERROR);
		return -1;
	}

	D3D11_TEXTURE2D_DESC t2desc;
	pTexture->GetDesc(&t2desc);
	pTexture->Release();

	g_TextureMap[pFilename] = g_NextTextureID;
	g_TextureIDMap[g_NextTextureID] = pFilename;
	g_TextureData.emplace(g_NextTextureID, Texture(pTextureView, t2desc.Width, t2desc.Height));

	if (pResource) {
		pResource->Release();
		pResource = nullptr;
	}
	
	return g_NextTextureID++;
}

void Texture_Release(int texid)
{
	if (texid < 0) return;
	auto it = g_TextureData.find(texid);
	if (it != g_TextureData.end()) {
		g_TextureData.erase(it);
	}
	auto it_id = g_TextureIDMap.find(texid);
	if (it_id != g_TextureIDMap.end()) {
		g_TextureMap.erase(it_id->second);
		g_TextureIDMap.erase(it_id);
	}
}

void Texture_AllRelease()
{
	g_NextTextureID = 0;
	g_TextureData.clear();
	g_TextureMap.clear();
	g_TextureIDMap.clear();
}

void Texture_SetTexture(int texid, int slot)
{
	ID3D11DeviceContext* pContext = Direct3D_GetDeviceContext();
	ID3D11ShaderResourceView* pNullTexture = nullptr;

	if (texid < 0) {
		pContext->PSSetShaderResources(slot, 1, &pNullTexture);
		return;
	}

	// テクスチャ設定
	auto it = g_TextureData.find(texid);
	if (it != g_TextureData.end() && it->second.pTexture != nullptr) {
		pContext->PSSetShaderResources(slot, 1, &it->second.pTexture);
	}
	else {
		pContext->PSSetShaderResources(slot, 1, &pNullTexture);
	}
}

unsigned int Texture_Width(int texid)
{
	if (texid < 0) return 0;

	auto it = g_TextureData.find(texid);
	if (it == g_TextureData.end()) {
		return 0;
	}

	return it->second.width;
}

unsigned int Texture_Height(int texid)
{
	if (texid < 0) return 0;

	auto it = g_TextureData.find(texid);
	if (it == g_TextureData.end()) {
		return 0;
	}

	return it->second.height;
}
