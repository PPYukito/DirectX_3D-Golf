/*==============================================================================

   3D描画用ピクセルシェーダー [shader_pixel_3d.hlsl]
														 Author : Youhei Sato
														 Date   : 2026/04/22
--------------------------------------------------------------------------------

==============================================================================*/
cbuffer LightBuffer : register(b0)
{
    float4 direction;
    float4 light_color;
    float4 ambient_color;
};

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float4 normalW : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0); // テクスチャ(tレジスタ)
SamplerState samp : register(s0); // サンプラ(sレジスタ)

float4 main(PS_INPUT ps_in) : SV_TARGET
{
    float4 normalW = normalize(ps_in.normalW);
    float4 directionW = normalize(direction);
   //  float l = max(dot(normalW.xyz, -directionW.xyz), 0.0f);
    float l = dot(normalW.xyz, -directionW.xyz) * 0.5f + 0.5f;
    
    float3 diffuse = light_color.rgb * l;
    float3 material = tex.Sample(samp, ps_in.uv).rgb * ps_in.color.rgb;
    float alpha = tex.Sample(samp, ps_in.uv).a * ps_in.color.a;
    
    return float4(material * (diffuse + ambient_color.rgb), alpha);
}
