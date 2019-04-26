#pragma once

#include "Flag.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix.h"

typedef struct Engine_DLL _tagResolution
{
	int Width;
	int Height;
}Resolution, *pResolution;

typedef struct Engine_DLL _tagColorVertex
{
	Vec3 Pos;
	Vec4 Color;

	_tagColorVertex()
	{

	}

	_tagColorVertex(const _tagColorVertex& _Other) : Pos(_Other.Pos), Color(_Other.Color)
	{
		
	}

	_tagColorVertex(const Vec3& _Pos, const Vec4& _Color) : Pos(_Pos), Color(_Color)
	{

	}
}ColorVertex, *PColorVertex;

typedef struct Engine_DLL _tagTransformConstBuffer
{
	Matrix World;
	Matrix View;
	Matrix Projection;
	Matrix WV;
	Matrix WVP;
}TransformConstBuffer, *PTransformConstBuffer;