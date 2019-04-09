#pragma once

#include "EngineMath.h"

typedef union Engine_DLL _tagVec4
{
	struct
	{
		float x, y, z, w;
	};

	struct
	{
		float r, g, b, a;
	};

	_tagVec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{

	}

	_tagVec4(const _tagVec4& _Other)
	{
		x = _Other.x; y = _Other.y; z = _Other.z; w = _Other.w;
	}

	_tagVec4(const DirectX::XMVECTOR& _Vec)
	{
		DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, _Vec);
	}

	_tagVec4(const DirectX::XMFLOAT4& _Vec)
	{
		x = _Vec.x; y = _Vec.y; z = _Vec.z; w = _Vec.w;
	}

	_tagVec4(int _x, int _y, int _z, int _w)
	{
		x = (float)_x; y = (float)_y; z = (float)z; w = (float)_w;
	}

	_tagVec4(float _x, float _y, float _z, float _w)
	{
		x = _x; y = _y; z = _z; w = _w;
	}

	_tagVec4(float _Value)
	{
		x = _Value; y = _Value; z = _Value; w = _Value;
	}

	_tagVec4(int _Value)
	{
		x = (float)_Value; y = (float)_Value; z = (float)_Value; w = (float)_Value;
	}

	_tagVec4(float _ArrValue[4])
	{
		x = _ArrValue[0];
		y = _ArrValue[1];
		z = _ArrValue[2];
		w = _ArrValue[3];
	}

	_tagVec4(int _ArrValue[4])
	{
		x = (float)_ArrValue[0];
		y = (float)_ArrValue[1];
		z = (float)_ArrValue[2];
		w = (float)_ArrValue[3];
	}

	float& operator[] (unsigned int _Idx);
	void operator= (const _tagVec4& _Other);
	void operator= (const DirectX::XMVECTOR& _Vec);
	void operator= (const DirectX::XMFLOAT4& _Vec);
	void operator= (float _Value);
	void operator= (int _Value);
	void operator= (float _ArrValue[4]);
	void operator= (int _ArrValue[4]);
	_tagVec4 operator+ (const _tagVec4& _Other) const;
	_tagVec4 operator+ (const DirectX::XMVECTOR& _Vec) const;
	_tagVec4 operator+ (const DirectX::XMFLOAT4& _Vec) const;
	_tagVec4 operator+ (float _Value) const;
	_tagVec4 operator+ (int _Value) const;
	_tagVec4 operator+ (float _ArrValue[4]) const;
	_tagVec4 operator+ (int _ArrValue[4]) const;
	void operator+= (const _tagVec4& _Other);
	void operator+= (const DirectX::XMVECTOR& _Vec);
	void operator+= (const DirectX::XMFLOAT4& _Vec);
	void operator+= (float _Value);
	void operator+= (int _Value);
	void operator+= (float _ArrValue[4]);
	void operator+= (int _ArrValue[4]);
	_tagVec4 operator- (const _tagVec4& _Other) const;
	_tagVec4 operator- (const DirectX::XMVECTOR& _Vec) const;
	_tagVec4 operator- (const DirectX::XMFLOAT4& _Vec) const;
	_tagVec4 operator- (float _Value) const;
	_tagVec4 operator- (int _Value) const;
	_tagVec4 operator- (float _ArrValue[4]) const;
	_tagVec4 operator- (int _ArrValue[4]) const;
	void operator-= (const _tagVec4& _Other);
	void operator-= (const DirectX::XMVECTOR& _Vec);
	void operator-= (const DirectX::XMFLOAT4& _Vec);
	void operator-= (float _Value);
	void operator-= (int _Value);
	void operator-= (float _ArrValue[4]);
	void operator-= (int _ArrValue[4]);
	_tagVec4 operator* (const _tagVec4& _Other) const;
	_tagVec4 operator* (const DirectX::XMVECTOR& _Vec) const;
	_tagVec4 operator* (const DirectX::XMFLOAT4& _Vec) const;
	_tagVec4 operator* (float _Value) const;
	_tagVec4 operator* (int _Value) const;
	_tagVec4 operator* (float _ArrValue[4]) const;
	_tagVec4 operator* (int _ArrValue[4]) const;
	void operator*= (const _tagVec4& _Other);
	void operator*= (const DirectX::XMVECTOR& _Vec);
	void operator*= (const DirectX::XMFLOAT4& _Vec);
	void operator*= (float _Value);
	void operator*= (int _Value);
	void operator*= (float _ArrValue[4]);
	void operator*= (int _ArrValue[4]);
	_tagVec4 operator/ (const _tagVec4& _Other) const;
	_tagVec4 operator/ (const DirectX::XMVECTOR& _Vec) const;
	_tagVec4 operator/ (const DirectX::XMFLOAT4& _Vec) const;
	_tagVec4 operator/ (float _Value) const;
	_tagVec4 operator/ (int _Value) const;
	_tagVec4 operator/ (float _ArrValue[4]) const;
	_tagVec4 operator/ (int _ArrValue[4]) const;
	void operator/= (const _tagVec4& _Other);
	void operator/= (const DirectX::XMVECTOR& _Vec);
	void operator/= (const DirectX::XMFLOAT4& _Vec);
	void operator/= (float _Value);
	void operator/= (int _Value);
	void operator/= (float _ArrValue[4]);
	void operator/= (int _ArrValue[4]);
	bool operator== (const _tagVec4& _Other) const;
	bool operator== (const DirectX::XMVECTOR& _Vec) const;
	bool operator== (const DirectX::XMFLOAT4& _Vec) const;
	bool operator== (float _Value) const;
	bool operator== (int _Value) const;
	bool operator== (float _ArrValue[4]) const;
	bool operator== (int _ArrValue[4]) const;
	bool operator!= (const _tagVec4& _Other) const;
	bool operator!= (const DirectX::XMVECTOR& _Vec) const;
	bool operator!= (const DirectX::XMFLOAT4& _Vec) const;
	bool operator!= (float _Value) const;
	bool operator!= (int _Value) const;
	bool operator!= (float _ArrValue[4]) const;
	bool operator!= (int _ArrValue[4]) const;

	// 이 함수를 실행한 객체의 값을 DirectX::XMVECTOR 형식으로 변환해서 리턴해줌
	DirectX::XMVECTOR Convert() const;
	// 이 함수에 넣어준 DirectX::XMVECTOR를 Vec4형식으로 이 객체에 넣어줌
	void Convert(const DirectX::XMVECTOR& _Vec);
	static _tagVec4 Normalize(const _tagVec4& _Value);

	static _tagVec4 Zero;
	static _tagVec4 One;
	static _tagVec4 White;
	static _tagVec4 Black;
	static _tagVec4 Red;
	static _tagVec4 Green;
	static _tagVec4 Blue;
}Vec4, *PVec4;