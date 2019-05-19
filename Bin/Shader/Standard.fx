#include "ShaderStruct.fx"

TextureCube CubeTex : register(t10);

VS_Out_Color StandardColorVS(VS_In_Color _In)
{
    VS_Out_Color Out = (VS_Out_Color) 0.0f;

    Out.Pos = mul(float4(_In.Pos, 1.0f), WVP);
    Out.Color = _In.Color;

	return Out;
}

PS_Out StandardColorPS(VS_Out_Color _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    Out.Color = _In.Color;

	return Out;
}

VS_Out_ColorNormal StandardColorNormalVS(VS_In_ColorNormal _In)
{
    VS_Out_ColorNormal Out = (VS_Out_ColorNormal) 0.0f;

    Out.ProjPos = mul(float4(_In.Pos, 1.0f), WVP);
    Out.Pos = Out.ProjPos;
    Out.Color = _In.Color;

    Out.ViewPos = mul(float4(_In.Pos, 1.0f), WV).xyz;
    Out.Normal = normalize(mul(float4(_In.Normal, 0.0f), WVRotation).xyz);

    return Out;
}

PS_Out_GBuffer StandardColorNormalPS(VS_Out_ColorNormal _In)
{
    PS_Out_GBuffer Out = (PS_Out_GBuffer) 0.0f;

    if (RenderMode == RENDER_FORWARD)
    {
        LightInfo Info = ComputeLight((float3) _In.ViewPos, _In.Normal);

        Out.Albedo = _In.Color * (Info.Dif + Info.Amb) + Info.Spc;
    }
    else
    {
        Out.Albedo = _In.Color;
        Out.Normal.xyz = _In.Normal;
        Out.Normal.w = MtrlSpc.w;
        Out.Depth.x = (_In.ProjPos.z / _In.ProjPos.w);
        Out.Depth.yz = (float2) 0.0f;
        Out.Depth.w = _In.ProjPos.w;
        Out.MaterialDif = MtrlDif;
        Out.MaterialAmb = MtrlAmb;
        Out.MaterialSpc = MtrlSpc;
        Out.MaterialEmv = MtrlEmv;
    }

    return Out;

}

VS_Out_Color StandardVertex3DVS(VS_In_3D _In)
{
    VS_Out_Color Out = (VS_Out_Color) 0.0f;

    return Out;
}

PS_Out StandardVertex3DPS(VS_Out_Color _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    return Out;
}

VS_Out_Sky SkyVS(VS_In_Pos _In)
{
    VS_Out_Sky Out = (VS_Out_Sky) 0.0f;

    Out.Pos = mul(float4(_In.Pos, 1.0f), WVP).xyww;
    Out.UV = _In.Pos;

    return Out;
}

PS_Out SkyPS(VS_Out_Sky _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    Out.Color = CubeTex.Sample(LinearSmp, _In.UV);

    return Out;

}

VS_Out_3D StandardTexNormalVS(VS_In_3D _In)
{
    VS_Out_3D Out = (VS_Out_3D) 0.0f;

    Out.ProjPos = mul(float4(_In.Pos, 1.0f), WVP);
    Out.Pos = Out.ProjPos;
    Out.UV = _In.UV;

    Out.ViewPos = mul(float4(_In.Pos, 1.0f), WV).xyz;

    Out.Normal = normalize(mul(float4(_In.Normal, 0.0f), WVRotation).xyz);

    return Out;
}

VS_Out_3D Standard3DVS(VS_In_3D _In)
{
    VS_Out_3D Out = (VS_Out_3D) 0.0f;

    _tagSkinning Tmp = (_tagSkinning) 0.0f;

    Tmp.Pos = _In.Pos;
    Tmp.Normal = _In.Normal;
    Tmp.Tangent = _In.Tangent;
    Tmp.Binormal = _In.Binormal;

    if (MtrlSkinning == 1)
    {
        if (MtrlNormal == 1)
        {
            Tmp = Skinning(_In.Pos, _In.Normal, _In.Tangent, _In.Binormal, _In.Weights, _In.Indices);
        }
        else
        {
            Tmp = Skinning(_In.Pos, _In.Normal, _In.Weights, _In.Indices);
        }
    }

    Out.ProjPos = mul(float4(Tmp.Pos, 1.0f), WVP);
    Out.Pos = Out.ProjPos;
    Out.UV = _In.UV;

    Out.ViewPos = mul(float4(Tmp.Pos, 1.0f), WV).xyz;

    Out.Normal = normalize(mul(float4(Tmp.Normal, 0.0f), WVRotation).xyz);

    if (MtrlNormal == 1)
    {
        Out.Tangent = normalize(mul(float4(Tmp.Tangent, 0.0f), WVRotation).xyz);
        Out.Binormal = normalize(mul(float4(Tmp.Binormal, 0.0f), WVRotation).xyz);
    }

    return Out;
}

PS_Out_GBuffer Standard3DPS(VS_Out_3D _In)
{
    PS_Out_GBuffer Out = (PS_Out_GBuffer) 0.0f;

    if (RenderMode == RENDER_FORWARD)
    {
        LightInfo Info = ComputeLight(_In.ViewPos, _In.Normal);

        float4 Color = DiffuseTex.Sample(LinearSmp, _In.UV);

        Out.Albedo = Color * (Info.Dif + Info.Amb) + Info.Spc;
    }

    else
    {
        float4 Color = DiffuseTex.Sample(LinearSmp, _In.UV);
        Out.Albedo = Color;

        float3 Normal = _In.Normal;

        if (MtrlNormal == 1)
        {
            float4 NormalCol = NormalTex.Sample(LinearSmp, _In.UV);
            Normal = NormalCol.xyz * 2.0f - 1.0f;

            float3x3 mat =
            {
                _In.Tangent,
                _In.Binormal,
                _In.Normal
            };

            Normal = normalize(mul(Normal, mat));
        }

        Out.Normal.xyz = Normal;
        Out.Normal.w = MtrlSpc.w;
        Out.Depth.x = (_In.ProjPos.z / _In.ProjPos.w);
        Out.Depth.yz = (float2) 0.0f;
        Out.Depth.w = _In.ProjPos.w;

        float4 Spc = MtrlSpc;

        if (MtrlSpecular == 1)
        {
            float4 Specular = SpecularTex.Sample(LinearSmp, _In.UV);

            Spc.xyz = Specular.xyz;
        }

        Out.MaterialDif = MtrlDif;
        Out.MaterialAmb = MtrlAmb;
        Out.MaterialSpc = Spc;
        Out.MaterialEmv = MtrlEmv;

    }

    return Out;
}