#ifndef DEPTH_STENCIL_STATE_H
#define DEPTH_STENCIL_STATE_H

#include <d3d11.h>

class DepthStencilState
{
public:
    enum class DepthStencilMode
    {
        None,
        Default,
        ReadOnly,
    };

private:
    ID3D11DepthStencilState* m_pDepthStencilState = nullptr;

public:
    DepthStencilState(DepthStencilMode mode = DepthStencilMode::Default);
    ~DepthStencilState();

    void Bind(UINT stencil_ref = 0);
};

#endif // DEPTH_STENCIL_STATE_H
