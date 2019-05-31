#include "ShaderStruct.fx"

cbuffer Button : register(b11)
{
    float4 ButtonColor;
}

VS_Out_UV ButtonVS(VS_In_UV _In)
{
    VS_Out_UV Out = (VS_Out_UV) 0.0f;

    Out.Pos = mul(float4(_In.Pos, 1.0f), WVP);
    Out.UV = _In.UV;

    return Out;
}

PS_Out ButtonPS(VS_Out_UV _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    Out.Color = DiffuseTex.Sample(LinearSmp, _In.UV) * ButtonColor;

    return Out;
}