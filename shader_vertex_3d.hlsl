/*==============================================================================

   3D描画用頂点シェーダー [shader_vertex_3d.hlsl]
														 Author : Youhei Sato
														 Date   : 2026/04/22
--------------------------------------------------------------------------------

==============================================================================*/

// 定数バッファ
cbuffer VSC0 : register(b0)
{
    float4x4 world;
};

cbuffer VSC1 : register(b1)
{
    float4x4 view;
};

cbuffer VSC2 : register(b2)
{
    float4x4 proj;
};

struct VS_IN
{
    float4 posL : POSITION0;
    float4 normalL : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 posH : SV_POSITION;
    float4 normalW : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

//=============================================================================
// 頂点シェーダ
//=============================================================================
VS_OUT main(VS_IN vs_in)
{
    VS_OUT ret;
    
    float4x4 mtxWV;
    float4x4 mtxWVP;
    
    mtxWV = mul(world, view);
    mtxWVP = mul(mtxWV, proj);
    
    ret.posH = mul(vs_in.posL, mtxWVP);
    ret.normalW = mul(float4(vs_in.normalL.xyz, 0.0f), world);
    ret.normalW = normalize(ret.normalW);
    ret.color = vs_in.color;
    ret.uv = vs_in.uv;
    
    return ret;
}
