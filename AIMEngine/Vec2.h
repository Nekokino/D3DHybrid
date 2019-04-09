#pragma once

#include "EngineMath.h"

typedef union Engine_DLL _tagVec2
{
	struct
	{
		float x, y;
	};

	_tagVec2() : x(0.0f), y(0.0f)
	{

	}

	_tagVec2(const _tagVec2& _Other)
	{
		x = _Other.x; y = _Other.y;
	}

	_tagVec2(const DirectX::XMVECTOR& _Vec)
	{
		DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, _Vec);
	}

	_tagVec2(const DirectX::XMFLOAT2& _Vec)
	{
		x = _Vec.x; y = _Vec.y;
	}

	_tagVec2(int _x, int _y)
	{
		x = (float)_x; y = (float)_y;
	}

	_tagVec2(float _x, float _y)
	{
		x = _x; y = _y;
	}

	_tagVec2(float _Value)
	{
		x = _Value; y = _Value;
	}

	_tagVec2(int _Value)
	{
		x = (float)_Value; y = (float)_Value;
	}

	_tagVec2(float _ArrValue[2])
	{
		x = _ArrValue[0];
		y = _ArrValue[1];
	}

	_tagVec2(int _ArrValue[2])
	{
		x = (float)_ArrValue[0];
		y = (float)_ArrValue[1];
	}

	float& operator[] (unsigned int _Idx);
	void operator= (const _tagVec2& _Other);
	void operator= (const DirectX::XMVECTOR& _Vec);
	void operator= (const DirectX::XMFLOAT2& _Vec);
	void operator= (float _Value);
	void operator= (int _Value);
	void operator= (float _ArrValue[2]);
	void operator= (int _ArrValue[2]);
	_tagVec2 operator+ (const _tagVec2& _Other) const;
	_tagVec2 operator+ (const DirectX::XMVECTOR& _Vec) const;
	_tagVec2 operator+ (const DirectX::XMFLOAT2& _Vec) const;
	_tagVec2 operator+ (float _Value) const;
	_tagVec2 operator+ (int _Value) const;
	_tagVec2 operator+ (float _ArrValue[2]) const;
	_tagVec2 operator+ (int _ArrValue[2]) const;
	void operator+= (const _tagVec2& _Other);
	void operator+= (const DirectX::XMVECTOR& _Vec);
	void operator+= (const DirectX::XMFLOAT2& _Vec);
	void operator+= (float _Value);
	void operator+= (int _Value);
	void operator+= (float _ArrValue[2]);
	void operator+= (int _ArrValue[2]);
	_tagVec2 operator- (const _tagVec2& _Other) const;
	_tagVec2 operator- (const DirectX::XMVECTOR& _Vec) const;
	_tagVec2 operator- (const DirectX::XMFLOAT2& _Vec) const;
	_tagVec2 operator- (float _Value) const;
	_tagVec2 operator- (int _Value) const;
	_tagVec2 operator- (float _ArrValue[2]) const;
	_tagVec2 operator- (int _ArrValue[2]) const;
	void operator-= (const _tagVec2& _Other);
	void operator-= (const DirectX::XMVECTOR& _Vec);
	void operator-= (const DirectX::XMFLOAT2& _Vec);
	void operator-= (float _Value);
	void operator-= (int _Value);
	void operator-= (float _ArrValue[2]);
	void operator-= (int _ArrValue[2]);
	_tagVec2 operator* (const _tagVec2& _Other) const;
	_tagVec2 operator* (const DirectX::XMVECTOR& _Vec) const;
	_tagVec2 operator* (const DirectX::XMFLOAT2& _Vec) const;
	_tagVec2 operator* (float _Value) const;
	_tagVec2 operator* (int _Value) const;
	_tagVec2 operator* (float _ArrValue[2]) const;
	_tagVec2 operator* (int _ArrValue[2]) const;
	void operator*= (const _tagVec2& _Other);
	void operator*= (const DirectX::XMVECTOR& _Vec);
	void operator*= (const DirectX::XMFLOAT2& _Vec);
	void operator*= (float _Value);
	void operator*= (int _Value);
	void operator*= (float _ArrValue[2]);
	void operator*= (int _ArrValue[2]);
	_tagVec2 operator/ (const _tagVec2& _Other) const;
	_tagVec2 operator/ (const DirectX::XMVECTOR& _Vec) const;
	_tagVec2 operator/ (const DirectX::XMFLOAT2& _Vec) const;
	_tagVec2 operator/ (float _Value) const;
	_tagVec2 operator/ (int _Value) const;
	_tagVec2 operator/ (float _ArrValue[2]) const;
	_tagVec2 operator/ (int _ArrValue[2]) const;
	void operator/= (const _tagVec2& _Other);
	void operator/= (const DirectX::XMVECTOR& _Vec);
	void operator/= (const DirectX::XMFLOAT2& _Vec);
	void operator/= (float _Value);
	void operator/= (int _Value);
	void operator/= (float _ArrValue[2]);
	void operator/= (int _ArrValue[2]);
	bool operator== (const _tagVec2& _Other) const;
	bool operator== (const DirectX::XMVECTOR& _Vec) const;
	bool operator== (const DirectX::XMFLOAT2& _Vec) const;
	bool operator== (float _Value) const;
	bool operator== (int _Value) const;
	bool operator== (float _ArrValue[2]) const;
	bool operator== (int _ArrValue[2]) const;
	bool operator!= (const _tagVec2& _Other) const;
	bool operator!= (const DirectX::XMVECTOR& _Vec) const;
	bool operator!= (const DirectX::XMFLOAT2& _Vec) const;
	bool operator!= (float _Value) const;
	bool operator!= (int _Value) const;
	bool operator!= (float _ArrValue[2]) const;
	bool operator!= (int _ArrValue[2]) const;

	float Length() const;

	// 이 함수를 실행한 객체의 값을 DirectX::XMVECTOR 형식으로 변환해서 리턴해줌
	DirectX::XMVECTOR Convert() const;
	// 이 함수에 넣어준 DirectX::XMVECTOR를 Vec2형식으로 이 객체에 넣어줌
	void Convert(const DirectX::XMVECTOR& _Vec);
	static _tagVec2 Normalize(const _tagVec2& _Value);

	static _tagVec2 Zero;
	static _tagVec2 One;
}Vec2, *PVec2;