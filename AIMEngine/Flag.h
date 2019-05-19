#pragma once

#include "Macro.h"

enum ShaderType
{
	ST_VTX,
	ST_PIX,
	ST_GEO,
	ST_END,
};

enum ComType
{
	CT_TRANSFORM,
	CT_RENDERER,
	CT_USERCOM,
	CT_CAMERA,
	CT_MATERIAL,
	CT_COLLIDER,
	CT_LIGHT,
	CT_UI,
	CT_LANDSCAPE,
	CT_ANIMATION,
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
	KS_PRESS,
	KS_PUSH,
	KS_RELEASE,
	KS_END,
};

enum SKEY
{
	SKEY_CTRL,
	SKEY_SHIFT,
	SKEY_ALT,
	SKEY_END,
};

enum LookAt_Axis
{
	LA_X,
	LA_Y,
	LA_Z,
	LA_ALL,
	LA_END,
};

enum CamType
{
	CT_PERS,
	CT_ORTH,
};

enum RenderStateType
{
	RS_RASTERIZER,
	RS_DEPTH,
	RS_BLEND,
	RS_END,
};

enum LightType
{
	LT_DIR,
	LT_POINT,
	LT_SPOT,
	LT_END,
};

enum RenderGroup
{
	RG_LANDSCAPE,
	RG_DEFAULT,
	RG_ALPHA,
	RG_HUD,
	RG_UI,
	RG_END,
};

enum RenderMode
{
	RM_FORWARD,
	RM_DEFERRED,
};

enum AnimationOption
{
	AO_LOOP,
	AO_ONCE_DESTROY,
};

enum ColliderType
{
	CT_SPHERE,
	CT_CAPSULE,
	CT_OBB,
	CT_POINT,
	CT_RAY,
	CT_AABB,
};

enum CollisionChannelState
{
	CCS_IGNORE,
	CCS_OVERLAP,
	CCS_BLOCK,
};

enum CollisionCallbackState
{
	CCS_BEGIN,
	CCS_STAY,
	CCS_LEAVE,
	CCS_END,
};