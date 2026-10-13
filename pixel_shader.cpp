/*==============================================================================

   ピクセルシェーダー [pixel_shader.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#include "pixel_shader.h"
#include "debug_ostream.h"
#include <fstream>

PixelShader::PixelShader(const std::string& filename)
	: m_pPixelShader(nullptr)
{
	// デバイスの取得
	ID3D11Device* pDevice = Direct3D_GetDevice();
	assert(pDevice && "Device is nullptr");

	// 事前コンパイル済みピクセルシェーダーの読み込み
	std::ifstream ifs(filename, std::ios::binary);
	assert(ifs && ("Failed to open shader file: " + filename).c_str());

	// ファイルサイズを取得
	ifs.seekg(0, std::ios::end);
	std::streamsize filesize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	// バイナリデータを格納するためのバッファを確保
	unsigned char* psbinary_pointer = new unsigned char[filesize];
	ifs.read((char*)psbinary_pointer, filesize);
	ifs.close();

	// ピクセルシェーダーの作成
	HRESULT hr = pDevice->CreatePixelShader(psbinary_pointer, filesize, nullptr, &m_pPixelShader);
	assert(SUCCEEDED(hr) && "Failed to create pixel shader");

	delete[] psbinary_pointer;
}

PixelShader::~PixelShader()
{
	if (m_pPixelShader) {
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}
}

void PixelShader::Bind()
{
	ID3D11DeviceContext* pContext = Direct3D_GetDeviceContext();
	assert(pContext && "DeviceContext is nullptr");

	if (m_pPixelShader) {
		pContext->PSSetShader(m_pPixelShader, nullptr, 0);
	}
}
