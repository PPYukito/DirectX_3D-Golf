/*==============================================================================

   入力レイアウト [input_layout.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H

#include <d3d11.h>
#include <cassert>
#include "direct3d.h"

class InputLayout
{
public:
    // コンストラクタ：入力レイアウトを作成
    InputLayout(const D3D11_INPUT_ELEMENT_DESC* pLayout, 
               UINT numElements, 
               const void* pShaderBytecode, 
               size_t bytecodeSize);

    // デストラクタ：入力レイアウトの解放
    ~InputLayout();

    // コピー禁止
    InputLayout(const InputLayout&) = delete;
    InputLayout& operator=(const InputLayout&) = delete;

    // 入力レイアウトを描画パイプラインに設定
    void Bind();

private:
    ID3D11InputLayout* m_pInputLayout;
};

#endif // INPUT_LAYOUT_H
