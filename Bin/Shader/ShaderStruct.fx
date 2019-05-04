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

struct PS_Out_GBuffer
{
    float4 Albedo : SV_TARGET;
    float4 Normal : SV_TARGET1;
    float4 Depth : SV_TARGET2;
    float4 MaterialDif : SV_TARGET3;
    float4 MaterialAmb : SV_TARGET4;
    float4 MaterialSpc : SV_TARGET5;
    float4 MaterialEmv : SV_TARGET6;
};

struct VS_In_UV
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_Out_UV
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_In_ColorNormal
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};

struct VS_Out_ColorNormal
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
    float3 ViewPos : POSITION;
    float4 ProjPos : POSITION1;
};

struct VS_3D_In
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float4 Weights : BLENDWEIGHTS;
    float4 Indices : BLENDINDICES;
};

struct VS_Pos_In
{
    float3 Pos : POSITION;
};

struct VS_Out_Sky
{
    float4 Pos : SV_POSITION;
    float3 UV : TEXCOORD;
};


cbuffer Transform : register(b0)
{
    matrix WorldRotation;
    matrix WVRotation;
    matrix World;
    matrix View;
    matrix Projection;
    matrix WV;
    matrix WVP;
}

cbuffer Material : register(b1)
{
    float4 MtrlDif;
    float4 MtrlAmb;
    float4 MtrlSpc;
    float4 MtrlEmv;
}

cbuffer Light : register(b2)
{
    float4 LightDif;
    float4 LightAmb;
    float4 LightSpc;
    float3 LightPos;
    int LightType;
    float3 LightDir;
    float LightDist;
    float LightInAngle;
    float LightOutAngle;
    float2 LightDummy;
}

cbuffer Render : register(b3)
{
    int RenderMode;
    float3 RenderDummy;
}

Texture2D DiffuseTex : register(t0);
SamplerState DiffuseSmp : register(s0);

struct LightInfo
{
    float4 Dif;
    float4 Amb;
    float4 Spc;
};

#define LIGHT_DIR 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

#define RENDER_FORWARD 0
#define REDNER_DEFERRED 1

LightInfo ComputeLight(float3 _ViewPos, float3 _ViewNormal)
{
    LightInfo Info = (LightInfo) 0;

    float3 LightDir = (float3) 0.0f;
    float Intensity = 1.0f;

    if (LightType == LIGHT_DIR)
    {
        LightDir = -normalize(mul(float4(LightDir, 0.0f), View).xyz);
    }

    if (LightType == LIGHT_POINT)
    {
        LightDir = LightPos - _ViewPos;
        LightDir = normalize(LightDir);

        float Dist = distance(LightPos, _ViewPos);

        if (Dist > LightDist)
        {
            Intensity = 0.0f;
        }
        else
        {
            Intensity = (1.0f - Dist / LightDist) * 0.7f + 0.3f;
        }
    }

    if (LightType == LIGHT_SPOT)
    {
        LightDir = LightPos - _ViewPos;
        LightDir = normalize(LightDir);

        float Dist = distance(LightPos, _ViewPos);

        if (Dist > LightDist)
        {
            Intensity = 0.0f;
        }
        else
        {
            float3 Dir = -LightDir;

            float Dot = dot(Dir, -LightDir);

            if (Dot < LightOutAngle)
            {
                Intensity = 0.0f;
            }
            else if (Dot >= LightInAngle)
            {
                Intensity = 1.0f;
            }

            else
            {
                Intensity = (LightInAngle - Dot) / (LightInAngle - LightOutAngle);
            }

            Intensity = Intensity * 0.3f + 0.7f;
            float DistIntensity = (1.0f - Dist / LightDist) * 0.3f + 0.7f;

            Intensity *= DistIntensity;

        }

    }

    Info.Dif = LightDif * MtrlDif * max(0, dot(_ViewNormal, LightDir)) * Intensity;
    Info.Amb = LightAmb * MtrlAmb;

    float3 R = 2.0f * dot(_ViewNormal, LightDir) * _ViewNormal - LightDir;
    R = normalize(R);

    float3 View = -normalize(_ViewPos);

    Info.Spc = LightSpc * MtrlSpc * pow(max(0, dot(R, View)), MtrlSpc.w) * Intensity;

    return Info;
}