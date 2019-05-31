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
Texture2D GBufferDepth : register(t11);

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
        Out.UV = ComputeFrameUV(UV[i]);

        _Stream.Append(Out);
    }

}

PS_Out ParticlePS(GS_Out_Particle _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    if(FrameAnimationType == AFT_ATLAS)
    {
        Out.Color = DiffuseTex.Sample(LinearSmp, _In.UV);
        Out.Color = FrameTex.Sample(LinearSmp, float3(_In.UV, FrameAnimationFrame));
    }
    else
    {
        Out.Color = FrameTex.Sample(LinearSmp, float3(_In.UV, FrameAnimationFrame));
    }

    if (Out.Color.a == 0.0f)
    {
        clip(-1);
    }

    float2 DepthUV = _In.ProjPos.xy / _In.ProjPos.w;
    DepthUV.x = DepthUV.x * 0.5f + 0.5f;
    DepthUV.y = DepthUV.y * -0.5f + 0.5f;

    float4 Depth = GBufferDepth.Sample(PointSmp, DepthUV);

    float DepthDist = Depth.w - _In.ProjPos.w;

    if (Depth.w == 0.0f)
    {
        DepthDist = 1.0f;
    }
    else if (DepthDist < 0.0f)
    {
        clip(-1);
    }
    else if (DepthDist == 0.0f)
    {
        DepthDist = 0.4f;
    }

    float Alpha = DepthDist / 0.4f;
    Alpha = min(Alpha, 1.0f);

    Out.Color.a *= Alpha;

    return Out;
}