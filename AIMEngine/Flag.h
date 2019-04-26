#pragma once

#include "Macro.h"

enum ShaderType
{
	ST_VTX,
	ST_PIX,
	ST_END,
};

enum ComType
{
	CT_TRANSFORM,
	CT_RENDERER,
	CT_USERCOM,
	CT_END,
};

enum Axis
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_END,
};

enum ConstShader
{
	CS_VERTEX = 0x1,
	CS_PIXEL = 0x2,
	CS_GEOMETRY = 0x4,
};

enum KeyState
{
	KS_PRESS = 0x1,
	KS_RELEASE = 0x2,
};