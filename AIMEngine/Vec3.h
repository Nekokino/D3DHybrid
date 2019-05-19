#pragma once

#include "EngineMath.h"

typedef union Engine_DLL _tagVec3
{
	struct
	{
		float x, y, z;
	};

	struct
	{
		float r, g, b;
	};

	_tagVec3() : x(0.0f), y(0.0f), z(0.0f)
	{

	}

	_tagVec3(const _tagVec3& _Other)
	{
		x = _Other.x; y = _Other.y; z = _Other.z;
	}

	_tagVec3(const DirectX::XMVECTOR& _Vec)
	{
		DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, _Vec);
	}

	_tagVec3(const DirectX::XMFLOAT3& _Vec)
	{
		x = _Vec.x; y = _Vec.y; z = _Vec.z;
	}

	_tagVec3(int _x, int _y, int _z)
	{
		x = (float)_x; y = (float)_y; z = (float)_z;
	}

	_tagVec3(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}

	_tagVec3(float _Value)
	{
		x = _Value; y = _Value; z = _Value;
	}

	_tagVec3(int _Value)
	{
		x = (float)_Value; y = (float)_Value; z = (float)_Value;
	}

	_tagVec3(float _ArrValue[3])
	{
		x = _ArrValue[0];
		y = _ArrValue[1];
		z = _ArrValue[2];
	}

	_tagVec3(int _ArrValue[3])
	{
		x = (float)_ArrValue[0];
		y = (float)_ArrValue[1];
		z = (float)_ArrValue[2];
	}

	float& operator[] (unsigned int _Idx);
	void operator= (const _tagVec3& _Other);
	void operator= (const DirectX::XMVECTOR& _Vec);
	void operator= (const DirectX::XMFLOAT3& _Vec);
	void operator= (float _Value);
	void operator= (int _Value);
	void operator= (float _ArrValue[3]);
	void operator= (int _ArrValue[3]);
	_tagVec3 operator+ (const _tagVec3& _Other) const;
	_tagVec3 operator+ (const DirectX::XMVECTOR& _Vec) const;
	_tagVec3 operator+ (const DirectX::XMFLOAT3& _Vec) const;
	_tagVec3 operator+ (float _Value) const;
	_tagVec3 operator+ (int _Value) const;
	_tagVec3 operator+ (float _ArrValue[3]) const;
	_tagVec3 operator+ (int _ArrValue[3]) const;
	void operator+= (const _tagVec3& _Other);
	void operator+= (const DirectX::XMVECTOR& _Vec);
	void operator+= (const DirectX::XMFLOAT3& _Vec);
	void operator+= (float _Value);
	void operator+= (int _Value);
	void operator+= (float _ArrValue[3]);
	void operator+= (int _ArrValue[3]);
	_tagVec3 operator- (const _tagVec3& _Other) const;
	_tagVec3 operator- (const DirectX::XMVECTOR& _Vec) const;
	_tagVec3 operator- (const DirectX::XMFLOAT3& _Vec) const;
	_tagVec3 operator- (float _Value) const;
	_tagVec3 operator- (int _Value) const;
	_tagVec3 operator- (float _ArrValue[3]) const;
	_tagVec3 operator- (int _ArrValue[3]) const;
	void operator-= (const _tagVec3& _Other);
	void operator-= (const DirectX::XMVECTOR& _Vec);
	void operator-= (const DirectX::XMFLOAT3& _Vec);
	void operator-= (float _Value);
	void operator-= (int _Value);
	void operator-= (float _ArrValue[3]);
	void operator-= (int _ArrValue[3]);
	_tagVec3 operator* (const _tagVec3& _Other) const;
	_tagVec3 operator* (const DirectX::XMVECTOR& _Vec) const;
	_tagVec3 operator* (const DirectX::XMFLOAT3& _Vec) const;
	_tagVec3 operator* (float _Value) const;
	_tagVec3 operator* (int _Value) const;
	_tagVec3 operator* (float _ArrValue[3]) const;
	_tagVec3 operator* (int _ArrValue[3]) const;
	void operator*= (const _tagVec3& _Other);
	void operator*= (const DirectX::XMVECTOR& _Vec);
	void operator*= (const DirectX::XMFLOAT3& _Vec);
	void operator*= (float _Value);
	void operator*= (int _Value);
	void operator*= (float _ArrValue[3]);
	void operator*= (int _ArrValue[3]);
	_tagVec3 operator/ (const _tagVec3& _Other) const;
	_tagVec3 operator/ (const DirectX::XMVECTOR& _Vec) const;
	_tagVec3 operator/ (const DirectX::XMFLOAT3& _Vec) const;
	_tagVec3 operator/ (float _Value) const;
	_tagVec3 operator/ (int _Value) const;
	_tagVec3 operator/ (float _ArrValue[3]) const;
	_tagVec3 operator/ (int _ArrValue[3]) const;
	void operator/= (const _tagVec3& _Other);
	void operator/= (const DirectX::XMVECTOR& _Vec);
	void operator/= (const DirectX::XMFLOAT3& _Vec);
	void operator/= (float _Value);
	void operator/= (int _Value);
	void operator/= (float _ArrValue[3]);
	void operator/= (int _ArrValue[3]);
	bool operator== (const _tagVec3& _Other) const;
	bool operator== (const DirectX::XMVECTOR& _Vec) const;
	bool operator== (const DirectX::XMFLOAT3& _Vec) const;
	bool operator== (float _Value) const;
	bool operator== (int _Value) const;
	bool operator== (float _ArrValue[3]) const;
	bool operator== (int _ArrValue[3]) const;
	bool operator!= (const _tagVec3& _Other) const;
	bool operator!= (const DirectX::XMVECTOR& _Vec) const;
	bool operator!= (const DirectX::XMFLOAT3& _Vec) const;
	bool operator!= (float _Value) const;
	bool operator!= (int _Value) const;
	bool operator!= (float _ArrValue[3]) const;
	bool operator!= (int _ArrValue[3]) const;

	float Length() const;
	float Distance(const _tagVec3& _Vec) const;
	void Normalize();
	float Dot(const _tagVec3& _Vec) const;
	float Dot(const DirectX::XMVECTOR& _Vec) const;
	float Dot(const DirectX::XMFLOAT3& _Vec) const;
	float Dot(float _ArrValue[3]) const;
	float Dot(int _ArrValue[3]) const;
	float Angle(const _tagVec3& _Vec) const;
	float Angle(const DirectX::XMVECTOR& _Vec) const;
	float Angle(const DirectX::XMFLOAT3& _Vec) const;
	float Angle(float _ArrValue[3]) const;
	float Angle(int _ArrValue[3]) const;
	_tagVec3 Cross(const _tagVec3& _Vec) const;
	_tagVec3 Cross(const DirectX::XMVECTOR& _Vec) const;
	_tagVec3 Cross(const DirectX::XMFLOAT3& _Vec) const;
	_tagVec3 Cross(float _ArrValue[3]) const;
	_tagVec3 Cross(int _ArrValue[3]) const;
	_tagVec3 TransformNormal(const union _tagMatrix& _Mat);
	_tagVec3 TransformNormal(const DirectX::XMMATRIX& _Mat);
	_tagVec3 TransformCoord(const union _tagMatrix& _Mat);
	_tagVec3 TransformCoord(const DirectX::XMMATRIX& _Mat);

	// 이 함수를 실행한 객체의 값을 DirectX::XMVECTOR 형식으로 변환해서 리턴해줌
	DirectX::XMVECTOR Convert() const;
	// 이 함수에 넣어준 DirectX::XMVECTOR를 Vec3형식으로 이 객체에 넣어줌
	void Convert(const DirectX::XMVECTOR& _Vec);
	static _tagVec3 Normalize(const _tagVec3& _Value);

	static _tagVec3 Axis[3];
	static _tagVec3 Zero;
	static _tagVec3 One;
}Vec3, *PVec3;