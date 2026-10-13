/*==============================================================================

   頂点シェーダー [vertex_shader.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include <d3d11.h>
#include <string>
#include <vector>
#include <cassert>
#include "direct3d.h"

class VertexShader
{
public:
	// コンストラクタ：ファイルから頂点シェーダーを作成
	explicit VertexShader(const std::string& filename);

	// デストラクタ：シェーダーの解放
	~VertexShader();

	// コピー禁止
	VertexShader(const VertexShader&) = delete;
	VertexShader& operator=(const VertexShader&) = delete;

	// シェーダーを描画パイプラインに設定
	void Bind();

	// バイトコードの取得（入力レイアウト作成用）
	const void* get_bytecode() const { return m_bytecode.data(); }
	size_t get_bytecode_size() const { return m_bytecode.size(); }

private:
	ID3D11VertexShader* m_pVertexShader;
	std::vector<unsigned char> m_bytecode;
};

#endif // VERTEX_SHADER_H
