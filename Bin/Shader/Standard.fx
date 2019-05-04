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
        Out.Normal.w = 1.0f;
        Out.Depth.x = (float3) (_In.ProjPos.z / _In.ProjPos.w);
        Out.Depth.yz = (float2) 0.0f;
        Out.Depth.w = 1.0f;
        Out.MaterialDif = MtrlDif;
        Out.MaterialAmb = MtrlAmb;
        Out.MaterialSpc = MtrlSpc;
        Out.MaterialEmv = MtrlEmv;

    }

    return Out;

}

VS_Out_Color StandardVertex3DVS(VS_3D_In _In)
{
    VS_Out_Color Out = (VS_Out_Color) 0.0f;

    return Out;
}

PS_Out StandardVertex3DPS(VS_Out_Color _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    return Out;
}

VS_Out_Sky SkyVS(VS_Pos_In _In)
{
    VS_Out_Sky Out = (VS_Out_Sky) 0.0f;

    Out.Pos = mul(float4(_In.Pos, 1.0f), WVP).xyww;
    Out.UV = _In.Pos;

    return Out;
}

PS_Out SkyPS(VS_Out_Sky _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    Out.Color = CubeTex.Sample(DiffuseSmp, _In.UV);

    return Out;

}