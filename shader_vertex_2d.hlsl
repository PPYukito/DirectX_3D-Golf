/*==============================================================================

   2D描画用頂点シェーダー [shader_vertex_2d.hlsl]
														 Author : Youhei Sato
														 Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/

// 定数バッファ
float4x4 mtx;

struct VS_IN
{
    float4 posL : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

//=============================================================================
// 頂点シェーダ
//=============================================================================
VS_OUT main(VS_IN vs_in)
{
    VS_OUT ret;
    ret.posH = mul(vs_in.posL, mtx);
    ret.color = vs_in.color;
    ret.uv = vs_in.uv;
    
    return ret;
}
