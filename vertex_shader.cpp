/*==============================================================================

   頂点シェーダー [vertex_shader.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#include "vertex_shader.h"
#include "debug_ostream.h"
#include <fstream>

VertexShader::VertexShader(const std::string& filename)
	: m_pVertexShader(nullptr)
{
	// デバイスの取得
	ID3D11Device* pDevice = Direct3D_GetDevice();
	assert(pDevice && "Device is nullptr");

	// 事前コンパイル済み頂点シェーダーの読み込み
	std::ifstream ifs(filename, std::ios::binary);
	assert(ifs && ("Failed to open shader file: " + filename).c_str());

	// ファイルサイズを取得
	ifs.seekg(0, std::ios::end);
	std::streamsize filesize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	// バイトコードを格納
	m_bytecode.resize(filesize);
	ifs.read((char*)m_bytecode.data(), filesize);
	ifs.close();

	// 頂点シェーダーの作成
	HRESULT hr = pDevice->CreateVertexShader(m_bytecode.data(), m_bytecode.size(), nullptr, &m_pVertexShader);
	assert(SUCCEEDED(hr) && "Failed to create vertex shader");
}

VertexShader::~VertexShader()
{
	if (m_pVertexShader) {
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}
	m_bytecode.clear();
}

void VertexShader::Bind()
{
	ID3D11DeviceContext* pContext = Direct3D_GetDeviceContext();
	assert(pContext && "DeviceContext is nullptr");

	if (m_pVertexShader) {
		pContext->VSSetShader(m_pVertexShader, nullptr, 0);
	}
}
