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

typedef struct Engine_DLL _tagVertexColorNormal
{
	Vec3 Pos;
	Vec3 Normal;
	Vec4 Color;

	_tagVertexColorNormal() {}
	_tagVertexColorNormal(const _tagVertexColorNormal& _Other) { *this = _Other; }
	_tagVertexColorNormal(const Vec3& _P, const Vec3& _N, const Vec4& _C) : Pos(_P), Normal(_N), Color(_C) {}
} VertexColorNormal, *PVertexColorNormal;

typedef struct Engine_DLL _tagVertexUV
{
	Vec3 Pos;
	Vec2 UV;

	_tagVertexUV() {}
	_tagVertexUV(const _tagVertexUV& _Other)
	{
		*this = _Other;
	}
	_tagVertexUV(const Vec3& _Pos, const Vec2& _UV) : Pos(_Pos), UV(_UV) {}
}VertexUV, *PVertexUV;

typedef struct Engine_DLL _tagTransformConstBuffer
{
	Matrix WorldRotation;
	Matrix WVRotation;
	Matrix World;
	Matrix View;
	Matrix Projection;
	Matrix InvProjection;
	Matrix WV;
	Matrix WVP;
	Matrix VP;
}TransformConstBuffer, *PTransformConstBuffer;

typedef struct Engine_DLL _tagMaterial
{
	Vec4 Diffuse;
	Vec4 Ambient;
	Vec4 Specular;
	Vec4 Emissive;
	int IsNormal;
	int IsSpecular;
	int Skinning;
	float Dump;

	_tagMaterial() : Diffuse(Vec4::White), Ambient(Vec4::White), Specular(Vec4::White), Emissive(Vec4::White), IsNormal(0), IsSpecular(0), Skinning(0)
	{
	}
}Material, *PMaterial;

typedef struct Engine_DLL _tagVertex3D
{
	Vec3 Pos;
	Vec3 Normal;
	Vec2 UV;
	Vec3 Tangent;
	Vec3 Binormal;
	Vec4 BlendWeights;
	Vec4 BlendIndices;
}Vertex3D, *PVertex3D;

typedef struct Engine_DLL _tagVertexDefaultParticle
{
	Vec3 Pos;
	Vec2 Size;

	_tagVertexDefaultParticle() {}
	_tagVertexDefaultParticle(const _tagVertexDefaultParticle& _Other)
	{
		*this = _Other;
	}
	_tagVertexDefaultParticle(const Vec3& _Pos, const Vec2& _Size) : Pos(_Pos), Size(_Size) {}
}VertexDefualtParticle, *PVertexDefaultParticle;

typedef struct Engine_DLL _tagLightInfo
{
	Vec4 Diffuse;
	Vec4 Ambient;
	Vec4 Specular;
	Vec3 Pos;
	int Type;
	Vec3 Direction;
	float Distance;
	float InAngle;
	float OutAngle;
	Vec2 Dump;

	_tagLightInfo() : Diffuse(Vec4::White), Ambient(Vec4(0.2f, 0.2f, 0.2f, 1.0f)), Specular(Vec4::White), Type(LT_DIR), Distance(0.0f), InAngle(0.0f), OutAngle(0.0f) {}
}LightInfo, *PLightInfo;

typedef struct Engine_DLL _tagRenderConstBuffer
{
	int RenderMode;
	Vec3 Dump;
}RenderConstBuffer, *PRenderConstBuffer;

typedef struct Engine_DLL _tagDebugConstBuffer
{
	Matrix WVP;
}DebugConstBuffer, *PDebugConstBuffer;

typedef struct Engine_DLL _tagParticleConstBuffer
{
	Vec3 Pos;
	float SizeX;
	Vec3 AxisX;
	float SizeY;
	Vec3 AxisY;
	float Dummy;
}ParticleConstBuffer, *PParticleConstBuffer;

typedef struct Engine_DLL _tagSphereInfo
{
	Vec3 Center;
	float Radius;

	_tagSphereInfo() : Radius(0.0f) {}
}SphereInfo, *PSphereInfo;

typedef struct Engine_DLL _tagAABBInfo
{
	Vec3 Min;
	Vec3 Max;
}AABBInfo, *PAABBInfo;

typedef struct Engine_DLL _tagOBBInfo
{
	Vec3 Center;
	Vec3 Axis[AXIS_END];
	Vec3 HalfLength;

	_tagOBBInfo()
	{
		for (int i = 0; i < AXIS_END; i++)
		{
			Axis[i] = Vec3::Axis[i];
			HalfLength = 1.0f;
		}
	}
}OBBInfo, *POBBInfo;