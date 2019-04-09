#include "Vec4.h"

// DirectX::XMVECTOR의 연산자 오버로딩을 사용하기 위해서는 DirectX 네임스페이스를 사용해줘야한다.
// 뭐 이런 요상한 조건이 다있지.
DXNamespace;

_tagVec4 _tagVec4::Zero = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
_tagVec4 _tagVec4::One = Vec4(0.0f, 0.0f, 0.0f, 0.0f);

_tagVec4 _tagVec4::Black = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
_tagVec4 _tagVec4::White = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
_tagVec4 _tagVec4::Red = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
_tagVec4 _tagVec4::Green = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
_tagVec4 _tagVec4::Blue = Vec4(0.0f, 0.0f, 1.0f, 1.0f);

float& _tagVec4::operator[] (unsigned int _Idx)
{
	switch (_Idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		break;
	}

	tassertmsg(true, "Index Over");

	return x;
}

void _tagVec4::operator= (const _tagVec4& _Other)
{
	x = _Other.x; y = _Other.y; z = _Other.z; w = _Other.w;
}

void _tagVec4::operator= (const DirectX::XMVECTOR& _Vec)
{
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, _Vec);
}

void _tagVec4::operator= (const DirectX::XMFLOAT4& _Vec)
{
	x = _Vec.x; y = _Vec.y; z = _Vec.z; w = _Vec.w;
}

void _tagVec4::operator= (float _Value)
{
	x = _Value; y = _Value; z = _Value; w = _Value;
}

void _tagVec4::operator= (int _Value)
{
	x = (float)_Value; y = (float)_Value; z = (float)_Value; w = (float)_Value;
}

void _tagVec4::operator= (float _ArrValue[4])
{
	x = _ArrValue[0];
	y = _ArrValue[1];
	z = _ArrValue[2];
	w = _ArrValue[3];
}

void _tagVec4::operator= (int _ArrValue[4])
{
	x = (float)_ArrValue[0];
	y = (float)_ArrValue[1];
	z = (float)_ArrValue[2];
	w = (float)_ArrValue[3];
}

_tagVec4 _tagVec4::operator+ (const _tagVec4& _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Other);

	Return = t1 + t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator+ (const DirectX::XMVECTOR& _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);

	Return = _Vec + t1;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator+ (const DirectX::XMFLOAT4& _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Vec);

	Return = t1 + t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator+ (float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_Value);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator+ (int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_Value);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator+ (float _ArrValue[4]) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator+ (int _ArrValue[4]) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 + t2;

	return _tagVec4(Return);
}

void _tagVec4::operator+= (const _tagVec4& _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = _Other.Convert();

	t1 += t2;

	Convert(t1);
}

void _tagVec4::operator+= (const DirectX::XMVECTOR& _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = Convert();

	t1 += _Vec;

	Convert(t1);
}

void _tagVec4::operator+= (const DirectX::XMFLOAT4& _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Vec);

	t1 += t2;

	Convert(t1);
}

void _tagVec4::operator+= (float _Value)
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_Value);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 += t2;

	Convert(t1);
}

void _tagVec4::operator+= (int _Value)
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_Value);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 += t2;

	Convert(t1);
}

void _tagVec4::operator+= (float _ArrValue[4])
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 += t2;

	Convert(t1);
}

void _tagVec4::operator+= (int _ArrValue[4])
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 += t2;

	Convert(t1);
}

_tagVec4 _tagVec4::operator- (const _tagVec4& _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Other);

	Return = t1 - t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator- (const DirectX::XMVECTOR& _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);

	Return = t1 - _Vec;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator- (const DirectX::XMFLOAT4& _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Vec);

	Return = t1 - t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator- (float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_Value);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator- (int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_Value);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator- (float _ArrValue[4]) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator- (int _ArrValue[4]) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 - t2;

	return _tagVec4(Return);
}

void _tagVec4::operator-= (const _tagVec4& _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = _Other.Convert();

	t1 -= t2;

	Convert(t1);
}

void _tagVec4::operator-= (const DirectX::XMVECTOR& _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = Convert();

	t1 -= _Vec;

	Convert(t1);
}

void _tagVec4::operator-= (const DirectX::XMFLOAT4& _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Vec);

	t1 -= t2;

	Convert(t1);
}

void _tagVec4::operator-= (float _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec4 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 -= t2;

	Convert(t1);
}

void _tagVec4::operator-= (int _Value)
{
	DirectX::XMVECTOR t1, t2;
	_tagVec4 tValue(_Value);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 -= t2;

	Convert(t1);
}

void _tagVec4::operator-= (float _ArrValue[4])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec4 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 -= t2;

	Convert(t1);
}

void _tagVec4::operator-= (int _ArrValue[4])
{
	DirectX::XMVECTOR t1, t2;
	_tagVec4 tValue(_ArrValue);
	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 -= t2;

	Convert(t1);
}

_tagVec4 _tagVec4::operator* (const _tagVec4& _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Other);

	Return = t1 * t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator* (const DirectX::XMVECTOR& _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);

	Return = _Vec * t1;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator* (const DirectX::XMFLOAT4& _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Vec);

	Return = t1 * t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator* (float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_Value);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator* (int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_Value);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator* (float _ArrValue[4]) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator* (int _ArrValue[4]) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 * t2;

	return _tagVec4(Return);
}

void _tagVec4::operator*= (const _tagVec4& _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = _Other.Convert();

	t1 *= t2;

	Convert(t1);
}

void _tagVec4::operator*= (const DirectX::XMVECTOR& _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = Convert();

	t1 *= _Vec;

	Convert(t1);
}

void _tagVec4::operator*= (const DirectX::XMFLOAT4& _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Vec);

	t1 *= t2;

	Convert(t1);
}

void _tagVec4::operator*= (float _Value)
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_Value);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 *= t2;

	Convert(t1);
}

void _tagVec4::operator*= (int _Value)
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_Value);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 *= t2;

	Convert(t1);
}

void _tagVec4::operator*= (float _ArrValue[4])
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 *= t2;

	Convert(t1);
}

void _tagVec4::operator*= (int _ArrValue[4])
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 *= t2;

	Convert(t1);
}

_tagVec4 _tagVec4::operator/ (const _tagVec4& _Other) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Other);

	Return = t1 / t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator/ (const DirectX::XMVECTOR& _Vec) const
{
	DirectX::XMVECTOR Return, t1;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);

	Return = _Vec / t1;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator/ (const DirectX::XMFLOAT4& _Vec) const
{
	DirectX::XMVECTOR Return, t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Vec);

	Return = t1 / t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator/ (float _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_Value);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator/ (int _Value) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_Value);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator/ (float _ArrValue[4]) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec4(Return);
}

_tagVec4 _tagVec4::operator/ (int _ArrValue[4]) const
{
	DirectX::XMVECTOR Return, t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = tValue.Convert();

	Return = t1 / t2;

	return _tagVec4(Return);
}

void _tagVec4::operator/= (const _tagVec4& _Other)
{
	DirectX::XMVECTOR t1, t2;

	t1 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
	t2 = _Other.Convert();

	t1 /= t2;

	Convert(t1);
}

void _tagVec4::operator/= (const DirectX::XMVECTOR& _Vec)
{
	DirectX::XMVECTOR t1;

	t1 = Convert();

	t1 /= _Vec;

	Convert(t1);
}

void _tagVec4::operator/= (const DirectX::XMFLOAT4& _Vec)
{
	DirectX::XMVECTOR t1, t2;

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&_Vec);

	t1 /= t2;

	Convert(t1);
}

void _tagVec4::operator/= (float _Value)
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_Value);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 /= t2;

	Convert(t1);
}

void _tagVec4::operator/= (int _Value)
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_Value);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 /= t2;

	Convert(t1);
}

void _tagVec4::operator/= (float _ArrValue[4])
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 /= t2;

	Convert(t1);
}

void _tagVec4::operator/= (int _ArrValue[4])
{
	DirectX::XMVECTOR t1, t2;

	_tagVec4 tValue(_ArrValue);

	t1 = Convert();
	t2 = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&tValue);

	t1 /= t2;

	Convert(t1);
}

bool _tagVec4::operator== (const _tagVec4& _Other) const
{
	return x == _Other.x && y == _Other.y && z == _Other.z && w == _Other.w;
}

bool _tagVec4::operator== (const DirectX::XMVECTOR& _Vec) const
{
	_tagVec4 t(_Vec);
	return x == t.x && y == t.y && z == t.z && w == t.w;
}

bool _tagVec4::operator== (const DirectX::XMFLOAT4& _Vec) const
{
	return x == _Vec.x && y == _Vec.y && z == _Vec.z && w == _Vec.w;
}

bool _tagVec4::operator== (float _Value) const
{
	return x == _Value && y == _Value && z == _Value && w == _Value;
}

bool _tagVec4::operator== (int _Value) const
{
	return x == _Value && y == _Value && z == _Value && w == _Value;
}

bool _tagVec4::operator== (float _ArrValue[4]) const
{
	return x == _ArrValue[0] && y == _ArrValue[1] && z == _ArrValue[2] && w == _ArrValue[3];
}

bool _tagVec4::operator== (int _ArrValue[4]) const
{
	return x == _ArrValue[0] && y == _ArrValue[1] && z == _ArrValue[2] && w == _ArrValue[3];
}

bool _tagVec4::operator!= (const _tagVec4& _Other) const
{
	return !(x == _Other.x && y == _Other.y && z == _Other.z && w == _Other.w);
}

bool _tagVec4::operator!= (const DirectX::XMVECTOR& _Vec) const
{
	_tagVec4 t(_Vec);
	return !(x == t.x && y == t.y && z == t.z && w == t.w);
}

bool _tagVec4::operator!= (const DirectX::XMFLOAT4& _Vec) const
{
	return !(x == _Vec.x && y == _Vec.y && z == _Vec.z && w == _Vec.w);
}

bool _tagVec4::operator!= (float _Value) const
{
	return !(x == _Value && y == _Value && z == _Value && w == _Value);
}

bool _tagVec4::operator!= (int _Value) const
{
	return !(x == _Value && y == _Value && z == _Value && w == _Value);
}

bool _tagVec4::operator!= (float _ArrValue[4]) const
{
	return !(x == _ArrValue[0] && y == _ArrValue[1] && z == _ArrValue[2] && w == _ArrValue[3]);
}

bool _tagVec4::operator!= (int _ArrValue[4]) const
{
	return !(x == _ArrValue[0] && y == _ArrValue[1] && z == _ArrValue[2] && w == _ArrValue[3]);
}

// 이 함수를 실행한 객체의 값을 DirectX::XMVECTOR 형식으로 변환해서 리턴해줌
DirectX::XMVECTOR _tagVec4::Convert() const
{
	return DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
}

// 이 함수에 넣어준 DirectX::XMVECTOR를 Vec4형식으로 이 객체에 넣어줌
void _tagVec4::Convert(const DirectX::XMVECTOR& _Vec)
{
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, _Vec);
}

_tagVec4 _tagVec4::Normalize(const _tagVec4& _Value)
{
	DirectX::XMVECTOR tValue = _Value.Convert();
	return _tagVec4(DirectX::XMVector4Normalize(tValue));
}