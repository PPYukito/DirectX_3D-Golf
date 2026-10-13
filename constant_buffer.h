/*==============================================================================

   定数バッファ [constant_buffer.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H

#include <d3d11.h>
#include <cassert>
#include "direct3d.h"

enum class ShaderType
{
	VertexShader,
	PixelShader
};

template<typename T>
class ConstantBuffer
{
public:
	// コンストラクタ：VS用またはPS用のバッファを作成、スロット指定
	ConstantBuffer(ShaderType type, UINT slot);

	// デストラクタ：バッファの解放
	~ConstantBuffer();

	// コピー禁止
	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&) = delete;

	// データを定数バッファに書き込み
	void Update(const T& data);

private:
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11DeviceContext* m_pContext;
	ShaderType m_type;
	UINT m_slot;
};

template<typename T>
ConstantBuffer<T>::ConstantBuffer(ShaderType type, UINT slot)
	: m_pConstantBuffer(nullptr), m_pContext(nullptr), m_type(type), m_slot(slot)
{
	// デバイスとコンテキストの取得
	ID3D11Device* pDevice = Direct3D_GetDevice();
	m_pContext = Direct3D_GetDeviceContext();

	assert(pDevice && "Device is nullptr");
	assert(m_pContext && "DeviceContext is nullptr");

	// 定数バッファの設定
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(T);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;

	// 定数バッファの作成
	HRESULT hr = pDevice->CreateBuffer(&buffer_desc, nullptr, &m_pConstantBuffer);
	assert(SUCCEEDED(hr) && "Failed to create constant buffer");
}

template<typename T>
ConstantBuffer<T>::~ConstantBuffer()
{
	if (m_pConstantBuffer) {
		m_pConstantBuffer->Release();
		m_pConstantBuffer = nullptr;
	}
}

template<typename T>
void ConstantBuffer<T>::Update(const T& data)
{
	assert(m_pContext && "DeviceContext is nullptr");
	assert(m_pConstantBuffer && "ConstantBuffer is nullptr");

	// 定数バッファを描画パイプラインに設定
	if (m_type == ShaderType::VertexShader) {
		m_pContext->VSSetConstantBuffers(m_slot, 1, &m_pConstantBuffer);
	}
	else {
		m_pContext->PSSetConstantBuffers(m_slot, 1, &m_pConstantBuffer);
	}
	
	m_pContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &data, 0, 0);
}

#endif // CONSTANT_BUFFER_H
