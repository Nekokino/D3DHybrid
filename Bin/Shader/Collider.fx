#include "ShaderStruct.fx"

cbuffer Collider : register(b10)
{
    float4 ColliderColor;
}

VS_Out_Pos ColliderVS(VS_In_Pos _In)
{
    VS_Out_Pos Out = (VS_Out_Pos) 0.0f;

    Out.Pos = mul(float4(_In.Pos, 1.0f), WVP);

    return Out;
}

PS_Out ColliderPS(VS_Out_Pos _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    Out.Color = ColliderColor;

    return Out;
}