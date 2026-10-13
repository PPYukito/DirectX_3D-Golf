#include "sampler_state.h"
#include "direct3d.h"


SamplerState::SamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE address_u, D3D11_TEXTURE_ADDRESS_MODE address_v, const float* border_color)
{
	// サンプラーステート設定
	D3D11_SAMPLER_DESC sampler_desc{};

	// フィルタリング
	sampler_desc.Filter = filter;

	// UV参照外の取り扱い（UVアドレッシングモード）
	sampler_desc.AddressU = address_u;
	sampler_desc.AddressV = address_v;

	if( sampler_desc.AddressU == D3D11_TEXTURE_ADDRESS_BORDER || sampler_desc.AddressV == D3D11_TEXTURE_ADDRESS_BORDER || border_color) {
		sampler_desc.BorderColor[0] = border_color[0];
		sampler_desc.BorderColor[1] = border_color[1];
		sampler_desc.BorderColor[2] = border_color[2];
		sampler_desc.BorderColor[3] = border_color[3];
	}
	else {
		sampler_desc.BorderColor[0] = 0.0f;
		sampler_desc.BorderColor[1] = 0.0f;
		sampler_desc.BorderColor[2] = 0.0f;
		sampler_desc.BorderColor[3] = 0.0f;
	}

	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.MipLODBias = 0;
	sampler_desc.MaxAnisotropy = 8; // max - 16
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	Direct3D_GetDevice()->CreateSamplerState(&sampler_desc, &m_pSamplerState);
}

SamplerState::~SamplerState()
{
	m_pSamplerState->Release();
}

void SamplerState::Bind(UINT slot)
{
	Direct3D_GetDeviceContext()->PSSetSamplers(slot, 1, &m_pSamplerState);
}
