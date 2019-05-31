#include "ShaderStruct.fx"

cbuffer LandScape : register(b10)
{
    float DetailLevelX;
    float DetailLevelZ;
    int SplattingCount;
    float Dummy;
}

Texture2DArray DiffuseSplattingTexture : register(t10);
Texture2DArray NormalSplattingTexture : register(t11);
Texture2DArray SpecularSplattingTexture : register(t12);
Texture2DArray AlphaSplattingTexture : register(t13);

VS_Out_3D LandScapeVS(VS_In_3D _In)
{
    VS_Out_3D Out = (VS_Out_3D) 0.0f;

    Out.ProjPos = mul(float4(_In.Pos, 1.0f), WVP);
    Out.Pos = Out.ProjPos;
    Out.UV = _In.UV;

    Out.ViewPos = mul(float4(_In.Pos, 1.0f), WV).xyz;

    Out.Normal = normalize(mul(float4(_In.Normal, 0.0f), WVRotation).xyz);
    Out.Tangent = normalize(mul(float4(_In.Tangent, 0.0f), WVRotation).xyz);
    Out.Binormal = normalize(mul(float4(_In.Binormal, 0.0f), WVRotation).xyz);
    return Out;
}

PS_Out_GBuffer LandScapePS(VS_Out_3D _In)
{
    PS_Out_GBuffer Out = (PS_Out_GBuffer) 0.0f;

    float2 DetailUV = _In.UV;
    DetailUV.x *= DetailLevelX;
    DetailUV.y *= DetailLevelZ;

    if (RenderMode == RENDER_FORWARD)
    {
        LightInfo Info = ComputeLight(_In.ViewPos, _In.Normal);

        float4 Color = DiffuseTex.Sample(LinearSmp, DetailUV);

        for (int i = 0; i < SplattingCount; ++i)
        {
            float4 Alpha = AlphaSplattingTexture.Sample(LinearSmp, float3(_In.UV, i));
            float4 SpDif = DiffuseSplattingTexture.Sample(LinearSmp, float3(DetailUV, i));

            Color = Color * (1.0f - Alpha.r) + SpDif * Alpha.r;

        }

        Out.Albedo = Color * (Info.Dif + Info.Amb) + Info.Spc;
    }
    else
    {
        float4 Color = DiffuseTex.Sample(LinearSmp, DetailUV);
        

        float3 Normal = _In.Normal;

        float4 NormalColor = NormalTex.Sample(LinearSmp, DetailUV);

        float4 Spc = MtrlSpc;

        float4 Specular = SpecularTex.Sample(LinearSmp, DetailUV);
        
        Spc.xyz = Specular.xyz;

        float4 Alpha = (float4) 0.0f;
        float4 SpDif = (float4) 0.0f;
        float4 SpNrm = (float4) 0.0f;
        float4 SpSpc = (float4) 0.0f;
        float Alpha2 = 0.0f;
        float InvAlpha = 0.0f;

        for (int i = 0; i < SplattingCount; ++i)
        {
            Alpha = AlphaSplattingTexture.Sample(LinearSmp, float3(_In.UV, i));
            SpDif = DiffuseSplattingTexture.Sample(LinearSmp, float3(DetailUV, i));
            SpNrm = NormalSplattingTexture.Sample(LinearSmp, float3(DetailUV, i));
            SpSpc = SpecularSplattingTexture.Sample(LinearSmp, float3(DetailUV, i));
        
            Alpha2 = Alpha.r;
            InvAlpha = 1.0f - Alpha.r;

            Color = Color * float4(InvAlpha, InvAlpha, InvAlpha, InvAlpha) + SpDif * float4(Alpha2, Alpha2, Alpha2, Alpha2);
            NormalColor = NormalColor * float4(InvAlpha, InvAlpha, InvAlpha, InvAlpha) + SpNrm * float4(Alpha2, Alpha2, Alpha2, Alpha2);
            Spc.xyz = Spc.xyz * float3(InvAlpha, InvAlpha, InvAlpha) + SpSpc.xyz * float3(Alpha2, Alpha2, Alpha2);

        }

        Normal = NormalColor.xyz * 2.0f - 1.0f;

        float3x3 mat =
        {
            _In.Tangent,
            _In.Binormal,
            _In.Normal
        };

        Normal = normalize(mul(Normal, mat));

        Out.Albedo = Color;

        Out.Normal.xyz = Normal;
        Out.Normal.w = MtrlSpc.w;
        Out.Depth.x = (_In.ProjPos.z / _In.ProjPos.w);
        Out.Depth.yz = (float2) 0.0f;
        Out.Depth.w = _In.ProjPos.w;

        Out.MaterialDif = MtrlDif;
        Out.MaterialAmb = MtrlAmb;
        Out.MaterialSpc = Spc;
        Out.MaterialEmv = MtrlEmv;
       
    }

   return Out;

}