/*==============================================================================

   ピクセルシェーダー [pixel_shader.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef PIXEL_SHADER_H
#define PIXEL_SHADER_H

#include <d3d11.h>
#include <string>
#include <cassert>
#include "direct3d.h"

class PixelShader
{
public:
	// コンストラクタ：ファイルからピクセルシェーダーを作成
	explicit PixelShader(const std::string& filename);

	// デストラクタ：シェーダーの解放
	~PixelShader();

	// コピー禁止
	PixelShader(const PixelShader&) = delete;
	PixelShader& operator=(const PixelShader&) = delete;

	// シェーダーを描画パイプラインに設定
	void Bind();

private:
	ID3D11PixelShader* m_pPixelShader;
};

#endif // PIXEL_SHADER_H
