#include "Matrix.h"
#include "Vec3.h"

DXNamespace;

_tagMatrix::_tagMatrix()
{
	m = DirectX::XMMatrixIdentity();
}

_tagMatrix::_tagMatrix(const _tagMatrix & _Other)
{
	m = _Other.m;
}

_tagMatrix::_tagMatrix(const DirectX::XMMATRIX & _m)
{
	m = _m;
}

void _tagMatrix::operator=(const _tagMatrix & _Other)
{
	m = _Other.m;
}

void _tagMatrix::operator=(const DirectX::XMMATRIX & _m)
{
	m = _m;
}

void _tagMatrix::operator=(float _f[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		v[i] = _f[i];
	}
}

Vec4 & _tagMatrix::operator[](unsigned int _Idx)
{
	return v[_Idx];
}

_tagMatrix _tagMatrix::operator*(const _tagMatrix & _Other) const
{
	return _tagMatrix(m * _Other.m);
}

_tagMatrix _tagMatrix::operator*(const DirectX::XMMATRIX & _m) const
{
	return _tagMatrix(m * _m);
}

_tagMatrix _tagMatrix::operator*(float _Value) const
{
	return _tagMatrix(m * _Value);
}

_tagMatrix _tagMatrix::operator*(int _Value) const
{
	return _tagMatrix(m * (float)_Value);
}

_tagMatrix _tagMatrix::operator*=(const _tagMatrix & _Other)
{
	m *= _Other.m;
	return *this;
}

_tagMatrix _tagMatrix::operator*=(const DirectX::XMMATRIX & _m)
{
	m *= _m;
	return *this;
}

_tagMatrix _tagMatrix::operator*=(float _Value)
{
	m *= _Value;
	return *this;
}

_tagMatrix _tagMatrix::operator*=(int _Value)
{
	m *= (float)_Value;
	return *this;
}

void * _tagMatrix::operator new(size_t _Size)
{
	//16바이트 정렬로 size만큼 힙에 할당하겠다는 것
	return _aligned_malloc(_Size, 16);
}

void _tagMatrix::operator delete(void * _p)
{
	_aligned_free(_p);
}

DirectX::XMMATRIX _tagMatrix::Identify()
{
	m = DirectX::XMMatrixIdentity();
	return m;
}

DirectX::XMMATRIX _tagMatrix::Transpose()
{
	m = DirectX::XMMatrixTranspose(m);
	return m;
}

DirectX::XMMATRIX _tagMatrix::Inverse()
{
	m = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(m), m);
	return m;
}

DirectX::XMMATRIX _tagMatrix::Scaling(float _x, float _y, float _z)
{
	m = DirectX::XMMatrixScaling(_x, _y, _z);
	return m;
}

DirectX::XMMATRIX _tagMatrix::Scaling(const _tagVec3 & _Scale)
{
	m = DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
	return m;
}

DirectX::XMMATRIX _tagMatrix::Rotation(float _x, float _y, float _z)
{
	DirectX::XMMATRIX MatRotX, MatRotY, MatRotZ;

	MatRotX = DirectX::XMMatrixRotationX(MATH_D2R * _x);
	MatRotY = DirectX::XMMatrixRotationX(MATH_D2R * _y);
	MatRotZ = DirectX::XMMatrixRotationX(MATH_D2R * _z);

	m = MatRotX * MatRotY * MatRotZ;
	return m;
}

DirectX::XMMATRIX _tagMatrix::Rotation(const _tagVec3 & _Rot)
{
	DirectX::XMMATRIX MatRotX, MatRotY, MatRotZ;

	MatRotX = DirectX::XMMatrixRotationX(MATH_D2R * _Rot.x);
	MatRotY = DirectX::XMMatrixRotationX(MATH_D2R * _Rot.y);
	MatRotZ = DirectX::XMMatrixRotationX(MATH_D2R * _Rot.z);

	m = MatRotX * MatRotY * MatRotZ;
	return m;
}

DirectX::XMMATRIX _tagMatrix::RotationX(float _x)
{
	m = DirectX::XMMatrixRotationX(MATH_D2R * _x);
	return m;
}

DirectX::XMMATRIX _tagMatrix::RotationY(float _y)
{
	m = DirectX::XMMatrixRotationX(MATH_D2R * _y);
	return m;
}

DirectX::XMMATRIX _tagMatrix::RotationZ(float _z)
{
	m = DirectX::XMMatrixRotationX(MATH_D2R * _z);
	return m;
}

DirectX::XMMATRIX _tagMatrix::RotationAxis(float _Angle, const _tagVec3 & _Axis)
{
	m = DirectX::XMMatrixRotationAxis(_Axis.Convert(), MATH_D2R * _Angle);
	return m;
}

DirectX::XMMATRIX _tagMatrix::Translation(float _x, float _y, float _z)
{
	m = DirectX::XMMatrixTranslation(_x, _y, _z);
	return m;
}

DirectX::XMMATRIX _tagMatrix::Translation(const _tagVec3 & _Pos)
{
	m = DirectX::XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);
	return m;
}
