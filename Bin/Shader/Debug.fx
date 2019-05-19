struct VS_In_UV
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_Out_UV
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct PS_Out
{
    float4 Color : SV_TARGET;
};

cbuffer Debug : register(b9)
{
    matrix DebugWVP;
}

Texture2D DiffuseTex : register(t0);
SamplerState DiffuseSmp : register(s0);

VS_Out_UV DebugVS(VS_In_UV _In)
{
    VS_Out_UV Out = (VS_Out_UV) 0.0f;

    Out.Pos = mul(float4(_In.Pos, 1.0f), DebugWVP);
    Out.UV = _In.UV;

    return Out;
}

PS_Out DebugPS(VS_Out_UV _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    float4 Color = DiffuseTex.Sample(LinearSmp, _In.UV);

    if (Color.a == 0.0f)
        clip(-1);

    Out.Color = Color;

    return Out;
}