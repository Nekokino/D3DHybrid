#include "Vec3.h"
#include "Matrix.h"

DXNamespace;

_tagVec3 _tagVec3::Axis[3] = { Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f) };
_tagVec3 _tagVec3::Zero = Vec3(0.0f, 0.0f, 0.0f);
_tagVec3 _tagVec3::One = Vec3(1.0f, 1.0f, 1.0f);

float & _tagVec3::operator[](unsigned int _Idx)
{
	switch (_Idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}

	tassertmsg(true, "Index Over");

	return x;
}

void _tagVec3::operator=(const _tagVec3 & _Other)
{
	x = _Other.x; y = _Other.y; z = _Other.z;
}

void _tagVec3::operator=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, _Vec);
}

void _tagVec3::operator=(const DirectX::XMFLOAT3 & _Vec)
{
	x = _Vec.x; y = _Vec.y; z = _Vec.z;
}

void _tagVec3::operator=(float _Value)
{
	x = _Value; y = _Value; z = _Value;
}

void _tagVec3::operator=(int _Value)
{
	x = (float)_Value; y = (float)_Value; z = (float)_Value;
}

void _tagVec3::operator=(float _ArrValue[3])
{
	x = _ArrValue[0]; y = _ArrValue[1]; z = _ArrValue[2];
}

void _tagVec3::operator=(int _ArrValue[3])
{
	x = (float)_ArrValue[0]; y = (float)_ArrValue[1]; z = (float)_ArrValue[2];
}

_tagVec3 _tagVec3::operator+(const _tagVec3 & _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&_Other);

	Return = t1 + t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator+(const DirectX::XMVECTOR & _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);

	Return = t1 + _Vec;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator+(const DirectX::XMFLOAT3 & _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = DirectX::XMLoadFloat3(&_Vec);

	Return = t1 + t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator+(float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_Value);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator+(int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_Value);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator+(float _ArrValue[3]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator+(int _ArrValue[3]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec3(Return);
}

void _tagVec3::operator+=(const _tagVec3 & _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = _Other.Convert();

	t1 += t2;

	Convert(t1);
}

void _tagVec3::operator+=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = Convert();

	t1 += _Vec;

	Convert(t1);
}

void _tagVec3::operator+=(const DirectX::XMFLOAT3 & _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&_Vec);

	t1 += t2;

	Convert(t1);
}

void _tagVec3::operator+=(float _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 += t2;

	Convert(t1);
}

void _tagVec3::operator+=(int _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 += t2;

	Convert(t1);
}

void _tagVec3::operator+=(float _ArrValue[3])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 += t2;

	Convert(t1);
}

void _tagVec3::operator+=(int _ArrValue[3])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 += t2;

	Convert(t1);
}

_tagVec3 _tagVec3::operator-(const _tagVec3 & _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&_Other);

	Return = t1 - t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator-(const DirectX::XMVECTOR & _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);

	Return = t1 - _Vec;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator-(const DirectX::XMFLOAT3 & _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = DirectX::XMLoadFloat3(&_Vec);

	Return = t1 - t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator-(float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_Value);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator-(int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_Value);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator-(float _ArrValue[3]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator-(int _ArrValue[3]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec3(Return);
}

void _tagVec3::operator-=(const _tagVec3 & _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = _Other.Convert();

	t1 -= t2;

	Convert(t1);
}

void _tagVec3::operator-=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = Convert();

	t1 -= _Vec;

	Convert(t1);
}

void _tagVec3::operator-=(const DirectX::XMFLOAT3 & _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&_Vec);

	t1 -= t2;

	Convert(t1);
}

void _tagVec3::operator-=(float _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 -= t2;

	Convert(t1);
}

void _tagVec3::operator-=(int _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 -= t2;

	Convert(t1);
}

void _tagVec3::operator-=(float _ArrValue[3])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 -= t2;

	Convert(t1);
}

void _tagVec3::operator-=(int _ArrValue[3])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 -= t2;

	Convert(t1);
}

_tagVec3 _tagVec3::operator*(const _tagVec3 & _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&_Other);

	Return = t1 * t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator*(const DirectX::XMVECTOR & _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);

	Return = t1 * _Vec;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator*(const DirectX::XMFLOAT3 & _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = DirectX::XMLoadFloat3(&_Vec);

	Return = t1 * t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator*(float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_Value);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator*(int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_Value);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator*(float _ArrValue[3]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator*(int _ArrValue[3]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec3(Return);
}

void _tagVec3::operator*=(const _tagVec3 & _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = _Other.Convert();

	t1 *= t2;

	Convert(t1);
}

void _tagVec3::operator*=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = Convert();

	t1 *= _Vec;

	Convert(t1);
}

void _tagVec3::operator*=(const DirectX::XMFLOAT3 & _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&_Vec);

	t1 *= t2;

	Convert(t1);
}

void _tagVec3::operator*=(float _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 *= t2;

	Convert(t1);
}

void _tagVec3::operator*=(int _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 *= t2;

	Convert(t1);
}

void _tagVec3::operator*=(float _ArrValue[3])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 *= t2;

	Convert(t1);
}

void _tagVec3::operator*=(int _ArrValue[3])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 *= t2;

	Convert(t1);
}

_tagVec3 _tagVec3::operator/(const _tagVec3 & _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&_Other);

	Return = t1 / t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator/(const DirectX::XMVECTOR & _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);

	Return = t1 / _Vec;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator/(const DirectX::XMFLOAT3 & _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = DirectX::XMLoadFloat3(&_Vec);

	Return = t1 / t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator/(float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_Value);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator/(int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_Value);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator/(float _ArrValue[3]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec3(Return);
}

_tagVec3 _tagVec3::operator/(int _ArrValue[3]) const
{
	DirectX::XMVECTOR Return, t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec3(Return);
}

void _tagVec3::operator/=(const _tagVec3 & _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
	t2 = _Other.Convert();

	t1 /= t2;

	Convert(t1);
}

void _tagVec3::operator/=(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = Convert();

	t1 /= _Vec;

	Convert(t1);
}

void _tagVec3::operator/=(const DirectX::XMFLOAT3 & _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&_Vec);

	t1 /= t2;

	Convert(t1);
}

void _tagVec3::operator/=(float _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 /= t2;

	Convert(t1);
}

void _tagVec3::operator/=(int _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 /= t2;

	Convert(t1);
}

void _tagVec3::operator/=(float _ArrValue[3])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 /= t2;

	Convert(t1);
}

void _tagVec3::operator/=(int _ArrValue[3])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec3 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&tValue);

	t1 /= t2;

	Convert(t1);
}

bool _tagVec3::operator==(const _tagVec3 & _Other) const
{
	return x == _Other.x && y == _Other.y && z == _Other.z;
}

bool _tagVec3::operator==(const DirectX::XMVECTOR & _Vec) const
{
	_tagVec3 T(_Vec);
	return x == T.x && y == T.y && z == T.z;
}

bool _tagVec3::operator==(const DirectX::XMFLOAT3 & _Vec) const
{
	return x == _Vec.x && y == _Vec.y && z == _Vec.z;
}

bool _tagVec3::operator==(float _Value) const
{
	return x == _Value && y == _Value && z == _Value;
}

bool _tagVec3::operator==(int _Value) const
{
	return x == _Value && y == _Value && z == _Value;
}

bool _tagVec3::operator==(float _ArrValue[3]) const
{
	return x == _ArrValue[0] && y == _ArrValue[1] && z == _ArrValue[2];
}

bool _tagVec3::operator==(int _ArrValue[3]) const
{
	return x == _ArrValue[0] && y == _ArrValue[1] && z == _ArrValue[2];
}

bool _tagVec3::operator!=(const _tagVec3 & _Other) const
{
	return !(x == _Other.x && y == _Other.y && z == _Other.z);
}

bool _tagVec3::operator!=(const DirectX::XMVECTOR & _Vec) const
{
	_tagVec3 T(_Vec);
	return !(x == T.x && y == T.y && z == T.z);
}

bool _tagVec3::operator!=(const DirectX::XMFLOAT3 & _Vec) const
{
	return !(x == _Vec.x && y == _Vec.y && z == _Vec.z);
}

bool _tagVec3::operator!=(float _Value) const
{
	return !(x == _Value && y == _Value && z == _Value);
}

bool _tagVec3::operator!=(int _Value) const
{
	return !(x == _Value && y == _Value && z == _Value);
}

bool _tagVec3::operator!=(float _ArrValue[3]) const
{
	return !(x == _ArrValue[0] && y == _ArrValue[1] && z == _ArrValue[2]);
}

bool _tagVec3::operator!=(int _ArrValue[3]) const
{
	return !(x == _ArrValue[0] && y == _ArrValue[1] && z == _ArrValue[2]);
}

float _tagVec3::Length() const
{
	DirectX::XMVECTOR t = Convert();

	return DirectX::XMVectorGetX(DirectX::XMVector3Length(t));
}

float _tagVec3::Distance(const _tagVec3 & _Vec) const
{
	return (*this - _Vec).Length();
}

void _tagVec3::Normalize()
{
	DirectX::XMVECTOR t = Convert();

	*this = DirectX::XMVector3Normalize(t);
}

float _tagVec3::Dot(const _tagVec3 & _Vec) const
{
	return Dot(_Vec.Convert());
}

float _tagVec3::Dot(const DirectX::XMVECTOR & _Vec) const
{
	return DirectX::XMVectorGetX(XMVector3Dot(Convert(), _Vec));
}

float _tagVec3::Dot(const DirectX::XMFLOAT3 & _Vec) const
{
	return Dot(DirectX::XMLoadFloat3(&_Vec));
}

float _tagVec3::Dot(float _ArrValue[3]) const
{
	_tagVec3 t(_ArrValue);
	return Dot(t.Convert());
}

float _tagVec3::Dot(int _ArrValue[3]) const
{
	_tagVec3 t(_ArrValue);
	return Dot(t.Convert());
}

float _tagVec3::Angle(const _tagVec3 & _Vec) const
{
	_tagVec3 t1 = Normalize(*this);
	_tagVec3 t2 = Normalize(_Vec);

	return MATH_R2D * acosf(t1.Dot(t2));
}

float _tagVec3::Angle(const DirectX::XMVECTOR & _Vec) const
{
	_tagVec3 t1 = Normalize(*this);
	_tagVec3 t2 = Normalize(_Vec);

	return MATH_R2D * acosf(t1.Dot(t2));
}

float _tagVec3::Angle(const DirectX::XMFLOAT3 & _Vec) const
{
	_tagVec3 t1 = Normalize(*this);
	_tagVec3 t2 = Normalize(_Vec);

	return MATH_R2D * acosf(t1.Dot(t2));
}

float _tagVec3::Angle(float _ArrValue[3]) const
{
	_tagVec3 t1 = Normalize(*this);
	_tagVec3 t2 = Normalize(_ArrValue);

	return MATH_R2D * acosf(t1.Dot(t2));
}

float _tagVec3::Angle(int _ArrValue[3]) const
{
	_tagVec3 t1 = Normalize(*this);
	_tagVec3 t2 = Normalize(_ArrValue);

	return MATH_R2D * acosf(t1.Dot(t2));
}

_tagVec3 _tagVec3::Cross(const _tagVec3 & _Vec) const
{
	return Cross(_Vec.Convert());
}

_tagVec3 _tagVec3::Cross(const DirectX::XMVECTOR & _Vec) const
{
	_tagVec3 t = _tagVec3(DirectX::XMVector3Cross(Convert(), _Vec));

	if (isnan(t.x) || isnan(t.y) || isnan(t.z))
	{
		return _tagVec3();
	}

	return t;
}

_tagVec3 _tagVec3::Cross(const DirectX::XMFLOAT3 & _Vec) const
{
	return Cross(DirectX::XMLoadFloat3(&_Vec));
}

_tagVec3 _tagVec3::Cross(float _ArrValue[3]) const
{
	_tagVec3 t(_ArrValue);
	return Cross(t.Convert());
}

_tagVec3 _tagVec3::Cross(int _ArrValue[3]) const
{
	_tagVec3 t(_ArrValue);
	return Cross(t.Convert());
}

_tagVec3 _tagVec3::TransformNormal(const _tagMatrix & _Mat)
{
	return _tagVec3(DirectX::XMVector3TransformNormal(Convert(), _Mat.m));
}

_tagVec3 _tagVec3::TransformNormal(const DirectX::XMMATRIX & _Mat)
{
	return _tagVec3(DirectX::XMVector3TransformNormal(Convert(), _Mat));
}

_tagVec3 _tagVec3::TransformCoord(const _tagMatrix & _Mat)
{
	return _tagVec3(DirectX::XMVector3TransformCoord(Convert(), _Mat.m));
}

_tagVec3 _tagVec3::TransformCoord(const DirectX::XMMATRIX & _Mat)
{
	return _tagVec3(DirectX::XMVector3TransformCoord(Convert(), _Mat));
}

DirectX::XMVECTOR _tagVec3::Convert() const
{
	return DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
}

void _tagVec3::Convert(const DirectX::XMVECTOR & _Vec)
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, _Vec);
}

_tagVec3 _tagVec3::Normalize(const _tagVec3 & _Value)
{
	DirectX::XMVECTOR t = _Value.Convert();
	return _tagVec3(DirectX::XMVector3Normalize(t));
}
