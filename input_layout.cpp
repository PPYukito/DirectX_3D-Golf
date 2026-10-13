/*==============================================================================

   入力レイアウト [input_layout.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#include "input_layout.h"
#include "debug_ostream.h"

InputLayout::InputLayout(const D3D11_INPUT_ELEMENT_DESC* pLayout, 
                         UINT numElements, 
                         const void* pShaderBytecode, 
                         size_t bytecodeSize)
    : m_pInputLayout(nullptr)
{
    // パラメータのチェック
    assert(pLayout && "Layout is nullptr");
    assert(numElements > 0 && "numElements is 0");
    assert(pShaderBytecode && "ShaderBytecode is nullptr");
    assert(bytecodeSize > 0 && "bytecodeSize is 0");

    // デバイスの取得
    ID3D11Device* pDevice = Direct3D_GetDevice();
    assert(pDevice && "Device is nullptr");

    // 入力レイアウトの作成
    HRESULT hr = pDevice->CreateInputLayout(pLayout, numElements, pShaderBytecode, bytecodeSize, &m_pInputLayout);
    assert(SUCCEEDED(hr) && "Failed to create input layout");
}

InputLayout::~InputLayout()
{
    if (m_pInputLayout) {
        m_pInputLayout->Release();
        m_pInputLayout = nullptr;
    }
}

void InputLayout::Bind()
{
    ID3D11DeviceContext* pContext = Direct3D_GetDeviceContext();
    assert(pContext && "DeviceContext is nullptr");

    if (m_pInputLayout) {
        pContext->IASetInputLayout(m_pInputLayout);
    }
}
