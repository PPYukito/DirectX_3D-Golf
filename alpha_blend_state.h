#ifndef ALPHA_BLEND_STATE_H
#define ALPHA_BLEND_STATE_H

#include <d3d11.h>

class AlphaBlendState
{
public:
    enum class BlendMode
    {
        None,
        Alpha,
        Additive,
        PremultipliedAlpha,
        Multiply,
    };

private:
    ID3D11BlendState* m_pBlendState = nullptr;

public:
    AlphaBlendState(BlendMode mode = BlendMode::Alpha);
    ~AlphaBlendState();

    void Bind(const float* blend_factor = nullptr, UINT sample_mask = 0xFFFFFFFF);
};

#endif // ALPHA_BLEND_STATE_H
