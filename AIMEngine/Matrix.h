#pragma once

#include "EngineMath.h"
#include "Vec4.h"

// __declspec(align(16)) -> �����͸� 16����Ʈ ���ķ� ���ڴٴ� �̴ϴ�.
typedef union Engine_DLL __declspec(align(16)) _tagMatrix
{
	DirectX::XMMATRIX m;

	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};

	struct
	{
		Vec4 v[4];
	};

	_tagMatrix();
	_tagMatrix(const _tagMatrix& _Other);
	_tagMatrix(const DirectX::XMMATRIX& _m);

	void operator= (const _tagMatrix& _Other);
	void operator= (const DirectX::XMMATRIX& _m);
	void operator= (float _f[4][4]);

	Vec4& operator[] (unsigned int _Idx);

	_tagMatrix operator* (const _tagMatrix& _Other) const;
	_tagMatrix operator* (const DirectX::XMMATRIX& _m) const;
	_tagMatrix operator* (float _Value) const;
	_tagMatrix operator* (int _Value) const;

	_tagMatrix operator*= (const _tagMatrix& _Other);
	_tagMatrix operator*= (const DirectX::XMMATRIX& _m);
	_tagMatrix operator*= (float _Value);
	_tagMatrix operator*= (int _Value);

	// 16����Ʈ ���Ķ����� new�� 16����Ʈ ������ �ǵ��� ������������Ѵ�.
	void* operator new (size_t _Size);
	// 16����Ʈ ���Ķ����� delete�� ������������Ѵ�. (���� 8����Ʈ ���İ��� �Ҵ�Ǵ� �޸��� ũ�Ⱑ �ణ �ٸ� ���̱� �����̴�.)
	void operator delete(void* _p);

	DirectX::XMMATRIX Identify();
	DirectX::XMMATRIX Transpose();
	DirectX::XMMATRIX Inverse();

	DirectX::XMMATRIX Scaling(float _x, float _y, float _z);
	DirectX::XMMATRIX Scaling(const union _tagVec3& _Scale);
	DirectX::XMMATRIX Rotation(float _x, float _y, float _z);
	DirectX::XMMATRIX Rotation(const union _tagVec3& _Rot);
	DirectX::XMMATRIX RotationX(float _x);
	DirectX::XMMATRIX RotationY(float _y);
	DirectX::XMMATRIX RotationZ(float _z);
	DirectX::XMMATRIX RotationAxis(float _Angle, const union _tagVec3& _Axis);
	DirectX::XMMATRIX Translation(float _x, float _y, float _z);
	DirectX::XMMATRIX Translation(const union _tagVec3& _Pos);
}Matrix, *PMatrix;

