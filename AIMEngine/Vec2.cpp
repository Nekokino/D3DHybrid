#include "Vec2.h"

DXNamespace;

_tagVec2 _tagVec2::One = Vec2(1.0f, 1.0f);
_tagVec2 _tagVec2::Zero = Vec2(0.0f, 0.0f);

float & _tagVec2::operator[](unsigned int _Idx)
{
	switch (_Idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	}

	tassertmsg(true, "Index Out of Bound");

	return x;
}

void _tagVec2::operator=(const _tagVec2 & _Other)
{
	x = _Other.x; y = _Other.y;
}

void _tagVec2::operator=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, _Vec);
}

void _tagVec2::operator=(const DirectX::XMFLOAT2 & _Vec)
{
	x = _Vec.x; y = _Vec.y;
}

void _tagVec2::operator=(float _Value)
{
	x = _Value; y = _Value;
}

void _tagVec2::operator=(int _Value)
{
	x = (float)_Value; y = (float)_Value;
}

void _tagVec2::operator=(float _ArrValue[2])
{
	x = _ArrValue[0]; y = _ArrValue[1];
}

void _tagVec2::operator=(int _ArrValue[2])
{
	x = (float)_ArrValue[0]; y = (float)_ArrValue[1];
}

_tagVec2 _tagVec2::operator+(const _tagVec2 & _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Other);

	Return = t1 + t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator+(const DirectX::XMVECTOR & _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);

	Return = t1 + _Vec;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator+(const DirectX::XMFLOAT2 & _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Vec);

	Return = t1 + t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator+(float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_Value);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator+(int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_Value);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator+(float _ArrValue[2]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator+(int _ArrValue[2]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec2(Return);
}

void _tagVec2::operator+=(const _tagVec2 & _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = _Other.Convert();

	t1 += t2;

	Convert(t1);
}

void _tagVec2::operator+=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);

	t1 += _Vec;

	Convert(t1);
}

void _tagVec2::operator+=(const DirectX::XMFLOAT2 & _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Vec);

	t1 += t2;

	Convert(t1);
}

void _tagVec2::operator+=(float _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 += t2;

	Convert(t1);
}

void _tagVec2::operator+=(int _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 += t2;

	Convert(t1);
}

void _tagVec2::operator+=(float _ArrValue[2])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 += t2;

	Convert(t1);
}

void _tagVec2::operator+=(int _ArrValue[2])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 += t2;

	Convert(t1);
}

_tagVec2 _tagVec2::operator-(const _tagVec2 & _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Other);

	Return = t1 - t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator-(const DirectX::XMVECTOR & _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);

	Return = t1 - _Vec;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator-(const DirectX::XMFLOAT2 & _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Vec);

	Return = t1 - t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator-(float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_Value);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator-(int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_Value);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator-(float _ArrValue[2]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator-(int _ArrValue[2]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec2(Return);
}

void _tagVec2::operator-=(const _tagVec2 & _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = _Other.Convert();

	t1 -= t2;

	Convert(t1);
}

void _tagVec2::operator-=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);

	t1 -= _Vec;

	Convert(t1);
}

void _tagVec2::operator-=(const DirectX::XMFLOAT2 & _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Vec);

	t1 -= t2;

	Convert(t1);
}

void _tagVec2::operator-=(float _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 -= t2;

	Convert(t1);
}

void _tagVec2::operator-=(int _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 -= t2;

	Convert(t1);
}

void _tagVec2::operator-=(float _ArrValue[2])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 -= t2;

	Convert(t1);
}

void _tagVec2::operator-=(int _ArrValue[2])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 -= t2;

	Convert(t1);
}

_tagVec2 _tagVec2::operator*(const _tagVec2 & _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Other);

	Return = t1 * t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator*(const DirectX::XMVECTOR & _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);

	Return = t1 * _Vec;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator*(const DirectX::XMFLOAT2 & _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Vec);

	Return = t1 * t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator*(float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_Value);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator*(int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_Value);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator*(float _ArrValue[2]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator*(int _ArrValue[2]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec2(Return);
}

void _tagVec2::operator*=(const _tagVec2 & _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = _Other.Convert();

	t1 *= t2;

	Convert(t1);
}

void _tagVec2::operator*=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);

	t1 *= _Vec;

	Convert(t1);
}

void _tagVec2::operator*=(const DirectX::XMFLOAT2 & _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Vec);

	t1 *= t2;

	Convert(t1);
}

void _tagVec2::operator*=(float _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 *= t2;

	Convert(t1);
}

void _tagVec2::operator*=(int _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 *= t2;

	Convert(t1);
}

void _tagVec2::operator*=(float _ArrValue[2])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 *= t2;

	Convert(t1);
}

void _tagVec2::operator*=(int _ArrValue[2])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 *= t2;

	Convert(t1);
}

_tagVec2 _tagVec2::operator/(const _tagVec2 & _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Other);

	Return = t1 / t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator/(const DirectX::XMVECTOR & _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);

	Return = t1 / _Vec;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator/(const DirectX::XMFLOAT2 & _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Vec);

	Return = t1 / t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator/(float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_Value);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator/(int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_Value);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator/(float _ArrValue[2]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec2(Return);
}

_tagVec2 _tagVec2::operator/(int _ArrValue[2]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec2 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec2(Return);
}

void _tagVec2::operator/=(const _tagVec2 & _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
	t2 = _Other.Convert();

	t1 /= t2;

	Convert(t1);
}

void _tagVec2::operator/=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);

	t1 /= _Vec;

	Convert(t1);
}

void _tagVec2::operator/=(const DirectX::XMFLOAT2 & _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&_Vec);

	t1 /= t2;

	Convert(t1);
}

void _tagVec2::operator/=(float _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 /= t2;

	Convert(t1);
}

void _tagVec2::operator/=(int _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 /= t2;

	Convert(t1);
}

void _tagVec2::operator/=(float _ArrValue[2])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 /= t2;

	Convert(t1);
}

void _tagVec2::operator/=(int _ArrValue[2])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec2 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)&tValue);

	t1 /= t2;

	Convert(t1);
}

bool _tagVec2::operator==(const _tagVec2 & _Other) const
{
	return x == _Other.x && y == _Other.y;
}

bool _tagVec2::operator==(const DirectX::XMVECTOR & _Vec) const
{
	_tagVec2 t(_Vec);
	return x == t.x && y == t.y;
}

bool _tagVec2::operator==(const DirectX::XMFLOAT2 & _Vec) const
{
	return x == _Vec.x && y == _Vec.y;
}

bool _tagVec2::operator==(float _Value) const
{
	return x == _Value && y == _Value;
}

bool _tagVec2::operator==(int _Value) const
{
	return x == _Value && y == _Value;
}

bool _tagVec2::operator==(float _ArrValue[2]) const
{
	return x == _ArrValue[0] && y == _ArrValue[1];
}

bool _tagVec2::operator==(int _ArrValue[2]) const
{
	return x == _ArrValue[0] && y == _ArrValue[1];
}

bool _tagVec2::operator!=(const _tagVec2 & _Other) const
{
	return !(x == _Other.x && y == _Other.y);
}

bool _tagVec2::operator!=(const DirectX::XMVECTOR & _Vec) const
{
	_tagVec2 t(_Vec);
	return !(x == t.x && y == t.y);
}

bool _tagVec2::operator!=(const DirectX::XMFLOAT2 & _Vec) const
{
	return !(x == _Vec.x && y == _Vec.y);
}

bool _tagVec2::operator!=(float _Value) const
{
	return !(x == _Value && y == _Value);
}

bool _tagVec2::operator!=(int _Value) const
{
	return !(x == _Value && y == _Value);
}

bool _tagVec2::operator!=(float _ArrValue[2]) const
{
	return !(x == _ArrValue[0] && y == _ArrValue[1]);
}

bool _tagVec2::operator!=(int _ArrValue[2]) const
{
	return !(x == _ArrValue[0] && y == _ArrValue[1]);
}

float _tagVec2::Length() const
{
	DirectX::XMVECTOR t = Convert();
	return DirectX::XMVectorGetX(DirectX::XMVector2Length(t));
}

DirectX::XMVECTOR _tagVec2::Convert() const
{
	return DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
}

void _tagVec2::Convert(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, _Vec);
}

_tagVec2 _tagVec2::Normalize(const _tagVec2 & _Value)
{
	DirectX::XMVECTOR t = _Value.Convert();
	return _tagVec2(DirectX::XMVector2Normalize(t));
}
