#include "ShaderStruct.fx"

Texture2D GBufferAlbedo : register(t10);
Texture2D GBufferNormal : register(t11);
Texture2D GBufferDepth : register(t12);
Texture2D GBufferMaterialDif : register(t13);
Texture2D GBufferMaterialAmb : register(t14);
Texture2D GBufferMaterialSpc : register(t15);
Texture2D GBufferMaterialEmv : register(t16);

Texture2D LightDiffuse : register(t17);
Texture2D LightSpecular : register(t18);

static const float2 NullPos[4] =
{
    float2(-1.0f, 1.0f),
    float2(1.0f, 1.0f),
    float2(-1.0f, -1.0f),
    float2(1.0f, -1.0f)
};

static const float2 NullUV[4] =
{
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 1.0f)
};

struct PS_Out_LightAcc
{
    float4 Dif : SV_TARGET;
    float4 Spc : SV_TARGET1;
};

VS_Out_UV LightAccDirVS(uint _VertexID : SV_VertexID)
{
    VS_Out_UV Out = (VS_Out_UV) 0.0f;

    Out.Pos = float4(NullPos[_VertexID], 0.0f, 1.0f);
    Out.UV = NullUV[_VertexID];

    return Out;
}

VS_Out_UV LightAccPointVS(VS_In_Pos _In)
{
    VS_Out_UV Out = (VS_Out_UV) 0.0f;

    float4 Pos = mul(float4(_In.Pos, 1.0f), WVP);
    Out.Pos = Pos;

    Out.UV.x = (Pos.x / Pos.w) * 0.5f + 0.5f;
    Out.UV.y = (Pos.y / Pos.w) * -0.5f + 0.5f;

    return Out;
}

VS_Out_UV LightAccSpotVS(uint _VertexID : SV_VertexID)
{
    VS_Out_UV Out = (VS_Out_UV) 0.0f;

    Out.Pos = float4(NullPos[_VertexID], 0.0f, 1.0f);
    Out.UV = NullUV[_VertexID];

    return Out;
}

PS_Out_LightAcc LightAccPS(VS_Out_UV _In)
{
    PS_Out_LightAcc Out = (PS_Out_LightAcc) 0.0f;

    float4 Depth = GBufferDepth.Sample(LinearSmp, _In.UV);

    if (Depth.w == 0.0f)
    {
        clip(-1);
    }

    float3 Pos = (float3) 0.0f;


    // UVÁÂÇ¥·Î w·Î ³ª´« ÁÂÇ¥ ±¸ÇÔ
    Pos.x = _In.UV.x * 2.0f - 1.0f;
    Pos.y = _In.UV.y * -2.0f + 1.0f;
    Pos.z = Depth.x;

    Pos *= Depth.w;

    Pos = mul(float4(Pos, Depth.w), InvProjection).xyz;

    float4 NormalColor = GBufferNormal.Sample(PointSmp, _In.UV);
    float3 Normal = NormalColor.xyz;

    float4 MaterialDif = GBufferMaterialDif.Sample(PointSmp, _In.UV);
    float4 MaterialAmb = GBufferMaterialAmb.Sample(PointSmp, _In.UV);
    float4 MaterialSpc = GBufferMaterialSpc.Sample(PointSmp, _In.UV);
    float4 MaterialEmv = GBufferMaterialEmv.Sample(PointSmp, _In.UV);

    LightInfo Info = ComputeLight(Pos, Normal, MaterialDif, MaterialAmb, MaterialSpc, MaterialEmv, NormalColor.w);

    Out.Dif = Info.Dif + Info.Amb;
    Out.Spc = Info.Spc + Info.Emv;

    return Out;
}

PS_Out LightBlendPS(VS_Out_UV _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    float4 Albedo = GBufferAlbedo.Sample(LinearSmp, _In.UV);
    float4 Dif = LightDiffuse.Sample(LinearSmp, _In.UV);
    float4 Spc = LightSpecular.Sample(LinearSmp, _In.UV);

    if(Albedo.a == 0.0f)
        clip(-1);

    Out.Color = Albedo * Dif + Spc;
    Out.Color.a = Albedo.a;

    return Out;
}

PS_Out LightBlendRenderPS(VS_Out_UV _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    Out.Color = DiffuseTex.Sample(LinearSmp, _In.UV);


    return Out;
}