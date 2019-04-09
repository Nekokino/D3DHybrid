#include "ShaderStruct.fx"

VS_Out_Color StandardColorVS(VS_In_Color _In)
{
    VS_Out_Color Out = (VS_Out_Color) 0.0f;

    Out.Pos = float4(_In.Pos, 1.0f);
    Out.Color = _In.Color;

	return Out;
}

PS_Out StandardColorPS(VS_Out_Color _In)
{
    PS_Out Out = (PS_Out) 0.0f;

    Out.Color = _In.Color;

	return Out;
}