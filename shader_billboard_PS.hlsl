/*==============================================================================

   3D描画用ピクセルシェーダー [shader_pixel_3d.hlsl]
														 Author : Youhei Sato
														 Date   : 2026/04/22
--------------------------------------------------------------------------------

==============================================================================*/
struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0); // テクスチャ(tレジスタ)
SamplerState samp : register(s0); // サンプラ(sレジスタ)

float4 main(PS_INPUT ps_in) : SV_TARGET
{
    float4 color = tex.Sample(samp, ps_in.uv);
    if (color.a < 0.02)
    {
        discard;
    }
    
    return color;
}
