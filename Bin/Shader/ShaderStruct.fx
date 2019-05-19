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
    float4 Pos : SV_POSITION;
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

struct VS_In_3D
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float4 Weights : BLENDWEIGHTS;
    float4 Indices : BLENDINDICES;
};

struct VS_Out_3D
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float3 ViewPos : POSITION;
    float4 ProjPos : POSITION1;
};

struct VS_In_Pos
{
    float3 Pos : POSITION;
};

struct VS_Out_Pos
{
    float4 Pos : SV_POSITION;
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
    matrix InvProjection;
    matrix WV;
    matrix WVP;
    matrix VP;
}

cbuffer Material : register(b1)
{
    float4 MtrlDif;
    float4 MtrlAmb;
    float4 MtrlSpc;
    float4 MtrlEmv;
    int MtrlNormal;
    int MtrlSpecular;
    int MtrlSkinning;
    float MtrlDummy;
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
Texture2D NormalTex : register(t1);
Texture2D SpecularTex : register(t2);
Texture2D BoneTex : register(t3);

SamplerState LinearSmp : register(s0);
SamplerState PointSmp : register(s1);

struct LightInfo
{
    float4 Dif;
    float4 Amb;
    float4 Spc;
    float4 Emv;
};

#define LIGHT_DIR 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

#define RENDER_FORWARD 0
#define REDNER_DEFERRED 1

LightInfo ComputeLight(float3 _ViewPos, float3 _ViewNormal)
{
    LightInfo Info = (LightInfo) 0;

    float3 tLightDir = (float3) 0.0f;
    float Intensity = 1.0f;

    if (LightType == LIGHT_DIR)
    {
        tLightDir = LightDir;
    }

    if (LightType == LIGHT_POINT)
    {
        tLightDir = LightPos - _ViewPos;
        tLightDir = normalize(tLightDir);

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
        tLightDir = LightPos - _ViewPos;
        tLightDir = normalize(tLightDir);

        float Dist = distance(LightPos, _ViewPos);

        if (Dist > LightDist)
        {
            Intensity = 0.0f;
        }
        else
        {
            float3 Dir = -tLightDir;

            float Dot = dot(Dir, LightDir);

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

    Info.Dif = LightDif * MtrlDif * max(0, dot(_ViewNormal, tLightDir)) * Intensity;
    Info.Amb = LightAmb * MtrlAmb;

    float3 R = 2.0f * dot(_ViewNormal, tLightDir) * _ViewNormal - tLightDir;
    R = normalize(R);

    float3 View = -normalize(_ViewPos);

    Info.Spc = LightSpc * MtrlSpc * pow(max(0, dot(R, View)), MtrlSpc.w) * Intensity;

    return Info;
}

LightInfo ComputeLight(float3 _ViewPos, float3 _ViewNormal, float4 _Diffuse, float4 _Ambient, float4 _Specular, float4 _Emissive, float _SpecularPower)
{
    LightInfo Info = (LightInfo) 0;

    float3 tLightDir = (float3) 0.0f;
    float Intensity = 1.0f;

    if (LightType == LIGHT_DIR)
    {
        tLightDir = LightDir;
    }

    if (LightType == LIGHT_POINT)
    {
        tLightDir = LightPos - _ViewPos;
        tLightDir = normalize(tLightDir);

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
        tLightDir = LightPos - _ViewPos;
        tLightDir = normalize(tLightDir);

        float Dist = distance(LightPos, _ViewPos);

        if (Dist > LightDist)
        {
            Intensity = 0.0f;
        }
        else
        {
            float3 Dir = -tLightDir;

            float Dot = dot(Dir, LightDir);

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

    Info.Dif = LightDif * _Diffuse * max(0.0f, dot(_ViewNormal, tLightDir)) * Intensity;
    Info.Amb = LightAmb * _Ambient;

    float3 Reflection = 2.0f * dot(_ViewNormal, tLightDir) * _ViewNormal - tLightDir;
    Reflection = normalize(Reflection);

    float3 View = -normalize(_ViewPos);

    Info.Spc = LightSpc * _Specular * pow(max(0, dot(Reflection, View)), _SpecularPower) * Intensity;

    Info.Emv = _Specular * _Emissive;

    return Info;

}

struct _tagSkinning
{
    float3 Pos;
    float3 Normal;
    float3 Tangent;
    float3 Binormal;
};

matrix GetBoneMatrix(int _Idx)
{
    matrix BoneMat =
    {
        BoneTex.Load(int3(_Idx * 4, 0, 0)),
        BoneTex.Load(int3(_Idx * 4 + 1, 0, 0)),
        BoneTex.Load(int3(_Idx * 4 + 2, 0, 0)),
        BoneTex.Load(int3(_Idx * 4 + 3, 0, 0))
    };

    return BoneMat;
}

_tagSkinning Skinning(float3 _Pos, float3 _Normal, float3 _Tangent, float3 _Binormal, float4 _Weights, float4 _Indices)
{
    _tagSkinning Tmp = (_tagSkinning) 0.0f;

    float Weights[4];
    Weights[0] = _Weights.x;
    Weights[1] = _Weights.y;
    Weights[2] = _Weights.z;
    Weights[3] = 1.0f - _Weights.x - _Weights.y - _Weights.z;

    for (int i = 0; i < 4; ++i)
    {
        matrix BoneMat = GetBoneMatrix((int) _Indices[i]);

        Tmp.Pos += Weights[i] * mul(float4(_Pos, 1.0f), BoneMat).xyz;
        Tmp.Normal += Weights[i] * mul(float4(_Normal, 0.0f), BoneMat).xyz;
        Tmp.Tangent += Weights[i] * mul(float4(_Tangent, 0.0f), BoneMat).xyz;
        Tmp.Binormal += Weights[i] * mul(float4(_Binormal, 0.0f), BoneMat).xyz;
    }

    Tmp.Normal = normalize(Tmp.Normal);
    Tmp.Tangent = normalize(Tmp.Tangent);
    Tmp.Binormal = normalize(Tmp.Binormal);

    return Tmp;
}

_tagSkinning Skinning(float3 _Pos, float3 _Normal, float4 _Weights, float4 _Indices)
{
    _tagSkinning Tmp = (_tagSkinning) 0.0f;

    float Weights[4];
    Weights[0] = _Weights.x;
    Weights[1] = _Weights.y;
    Weights[2] = _Weights.z;
    Weights[3] = 1.0f - _Weights.x - _Weights.y - _Weights.z;

    for (int i = 0; i < 4; ++i)
    {
        matrix BoneMat = GetBoneMatrix((int) _Indices[i]);

        Tmp.Pos += Weights[i] * mul(float4(_Pos, 1.0f), BoneMat).xyz;
        Tmp.Normal += Weights[i] * mul(float4(_Normal, 0.0f), BoneMat).xyz;
    }

    Tmp.Normal = normalize(Tmp.Normal);

    return Tmp;

}