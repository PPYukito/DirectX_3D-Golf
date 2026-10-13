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

cbuffer VSC3 : register(b3)
{
    float4x4 mtxUV;
};

struct VS_IN
{
    float4 posL : POSITION0;
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 posH : SV_POSITION;
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
    ret.uv = mul(float4(vs_in.uv, 0.0f, 1.0f), mtxUV).xy;
    
    return ret;
}
