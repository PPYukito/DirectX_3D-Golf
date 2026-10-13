#pragma once

#include <d3d11.h>


class SamplerState
{
private:
	ID3D11SamplerState* m_pSamplerState = nullptr;

public:
	SamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE address_u = D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_MODE address_v = D3D11_TEXTURE_ADDRESS_CLAMP, const float* border_color = nullptr);
	~SamplerState();

	void Bind(UINT slot = 0);
};
