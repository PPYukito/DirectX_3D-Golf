#include "alpha_blend_state.h"
#include "direct3d.h"

AlphaBlendState::AlphaBlendState(BlendMode mode)
{
    D3D11_BLEND_DESC blend_desc{};
    blend_desc.AlphaToCoverageEnable = FALSE;
    blend_desc.IndependentBlendEnable = FALSE;

    D3D11_RENDER_TARGET_BLEND_DESC& rt_desc = blend_desc.RenderTarget[0];
    rt_desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    switch (mode)
    {
    case BlendMode::None:
        rt_desc.BlendEnable = FALSE;
        rt_desc.SrcBlend = D3D11_BLEND_ONE;
        rt_desc.DestBlend = D3D11_BLEND_ZERO;
        rt_desc.BlendOp = D3D11_BLEND_OP_ADD;
        rt_desc.SrcBlendAlpha = D3D11_BLEND_ONE;
        rt_desc.DestBlendAlpha = D3D11_BLEND_ZERO;
        rt_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        break;

    case BlendMode::Alpha:
		// src_rgb * src_alpha + dest_rgb * (1 - src_alpha) 
        rt_desc.BlendEnable = TRUE;
        rt_desc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        rt_desc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        rt_desc.BlendOp = D3D11_BLEND_OP_ADD;
        rt_desc.SrcBlendAlpha = D3D11_BLEND_ONE;
        rt_desc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        rt_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        break;

    case BlendMode::Additive:
        rt_desc.BlendEnable = TRUE;
        rt_desc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        rt_desc.DestBlend = D3D11_BLEND_ONE;
        rt_desc.BlendOp = D3D11_BLEND_OP_ADD;
        rt_desc.SrcBlendAlpha = D3D11_BLEND_ONE;
        rt_desc.DestBlendAlpha = D3D11_BLEND_ONE;
        rt_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        break;

    case BlendMode::PremultipliedAlpha:
        rt_desc.BlendEnable = TRUE;
        rt_desc.SrcBlend = D3D11_BLEND_ONE;
        rt_desc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        rt_desc.BlendOp = D3D11_BLEND_OP_ADD;
        rt_desc.SrcBlendAlpha = D3D11_BLEND_ONE;
        rt_desc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        rt_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        break;

    case BlendMode::Multiply:
        rt_desc.BlendEnable = TRUE;
        rt_desc.SrcBlend = D3D11_BLEND_DEST_COLOR;
        rt_desc.DestBlend = D3D11_BLEND_ZERO;
        rt_desc.BlendOp = D3D11_BLEND_OP_ADD;
        rt_desc.SrcBlendAlpha = D3D11_BLEND_ONE;
        rt_desc.DestBlendAlpha = D3D11_BLEND_ZERO;
        rt_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        break;
    }

    Direct3D_GetDevice()->CreateBlendState(&blend_desc, &m_pBlendState);
}

AlphaBlendState::~AlphaBlendState()
{
    SAFE_RELEASE(m_pBlendState);
}

void AlphaBlendState::Bind(const float* blend_factor, UINT sample_mask)
{
    const float default_blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    Direct3D_GetDeviceContext()->OMSetBlendState(m_pBlendState, blend_factor ? blend_factor : default_blend_factor, sample_mask);
}
