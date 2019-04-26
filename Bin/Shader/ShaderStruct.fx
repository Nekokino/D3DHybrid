struct VS_In_Color
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

struct VS_Out_Color
{
    float4 Pos : SV_POSITION;
    float4 Color : Color;
};

struct PS_Out
{
    float4 Color : SV_TARGET;
};

cbuffer Transform : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix WV;
    matrix WVP;
}