#include "ShaderStruct.fx"

struct VS_In_Particle
{
    float3 Pos : POSITION;
};

struct GS_Out_Particle
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
    float2 UV : TEXCOORD;
};

cbuffer Particle : register(b10)
{
    float3 ParticlePos;
    float SizeX;
    float3 AxisX;
    float SizeY;
    float3 AxisY;
    float ParticleDummy;
}

Texture2DArray FrameTex : register(t10);

VS_In_Particle ParticleVS(VS_In_Particle _In)
{
    return _In;
}

[maxvertexcount(4)]
void ParticleGS(point VS_In_Particle _In[1], inout TriangleStream<GS_Out_Particle> _Stream)
{
    float HalfX = SizeX * 0.5f;
    float HalfY = SizeY * 0.5f;

    float3 Pos[4];

    Pos[0] = ParticlePos - AxisX * HalfX - AxisY * HalfY;
    Pos[1] = ParticlePos - AxisX * HalfX + AxisY * HalfY;
    Pos[2] = ParticlePos + AxisX * HalfX - AxisY * HalfY;
    Pos[3] = ParticlePos + AxisX * HalfX + AxisY * HalfY;

    float2 UV[4] =
    {
        float2(0.0f, 1.0f),
        float2(0.0f, 0.0f),
        float2(1.0f, 1.0f),
        float2(1.0f, 0.0f)
    };

    GS_Out_Particle Out = (GS_Out_Particle) 0.0f;

    for (int i = 0; i < 4; ++i)
    {
        Out.ProjPos = mul(float4(Pos[i], 1.0f), VP);
        Out.Pos = Out.ProjPos;
        Out.UV = UV[i];

        _Stream.Append(Out);
    }

}

PS_Out ParticlePS(GS_Out_Particle _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    Out.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);

    return Out;
}