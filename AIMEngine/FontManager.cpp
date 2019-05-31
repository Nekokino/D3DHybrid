#include "FontManager.h"
#include "AIMDevice.h"

IDWriteFactory* FontManager::WriteFactory = nullptr;
std::unordered_map<std::string, IDWriteTextFormat*> FontManager::FontMap;
std::unordered_map<unsigned int, ID2D1SolidColorBrush*> FontManager::ColorMap;

bool FontManager::Init()
{
	// uuidof 매개변수에 연결된 GUID 찾는 함수


	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(WriteFactory), (IUnknown**)&WriteFactory)))
	{
		return false;
	}

	CreateTextFormat("Gungseo", TEXT("궁서체"), DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 20.0f, TEXT("ko"));
	CreateTextFormat("FreshGothic", TEXT("맑은 고딕"), DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 20.0f, TEXT("ko"));

	return true;
}

void FontManager::Release()
{
	std::unordered_map<std::string, IDWriteTextFormat*>::iterator StartIter = FontMap.begin();
	std::unordered_map<std::string, IDWriteTextFormat*>::iterator EndIter = FontMap.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		if (StartIter->second != nullptr)
		{
			StartIter->second->Release();
			StartIter->second = nullptr;
		}
	}

	std::unordered_map<unsigned int, ID2D1SolidColorBrush*>::iterator StartIter2 = ColorMap.begin();
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*>::iterator EndIter2 = ColorMap.end();

	for (; StartIter2 != EndIter2 ; ++StartIter2)
	{
		if (StartIter2->second != nullptr)
		{
			StartIter2->second->Release();
			StartIter2->second = nullptr;
		}
	}

	if (WriteFactory != nullptr)
	{
		WriteFactory->Release();
		WriteFactory = nullptr;
	}

}

bool FontManager::CreateTextFormat(const std::string & _Name, const TCHAR * _FontName, int _Weight, int _Style, int _Stretch, float _Size, const TCHAR * _LocalName)
{
	IDWriteTextFormat* Font = FindTextFormat(_Name);

	if (Font != nullptr)
	{
		return false;
	}

	// 폰트이름, 특정 폰트 컬렉션, 굵기, 기울기, 자간, 크기, 언어지역 이름, 텍스트 인터페이스
	if (FAILED(WriteFactory->CreateTextFormat(_FontName, nullptr, (DWRITE_FONT_WEIGHT)_Weight, (DWRITE_FONT_STYLE)_Style, (DWRITE_FONT_STRETCH)_Stretch, _Size, _LocalName, &Font)))
	{
		return false;
	}

	FontMap.insert(std::unordered_map<std::string, IDWriteTextFormat*>::value_type(_Name, Font));

	return true;
}

ID2D1SolidColorBrush * FontManager::CreateColor(float _r, float _g, float _b, float _a)
{
	ID2D1SolidColorBrush* Color = FindColor(_r, _g, _b, _a);

	if (Color != nullptr)
	{
		return Color;
	}

	if (FAILED(GetDeviceInst->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_r, _g, _b, _a), &Color)))
	{
		return nullptr;
	}

	ColorMap.insert(std::unordered_map<unsigned int, ID2D1SolidColorBrush*>::value_type(CreateColorKey(_r, _g, _b, _a), Color));

	return Color;
}

ID2D1SolidColorBrush * FontManager::CreateColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	ID2D1SolidColorBrush* Color = FindColor(_r, _g, _b, _a);

	if (Color != nullptr)
	{
		return Color;
	}

	if (FAILED(GetDeviceInst->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_r / 255.0f, _g / 255.0f, _b / 255.0f, _a / 255.0f), &Color)))
	{
		return nullptr;
	}

	ColorMap.insert(std::unordered_map<unsigned int, ID2D1SolidColorBrush*>::value_type(CreateColorKey(_r, _g, _b, _a), Color));

	return Color;
}

ID2D1SolidColorBrush * FontManager::CreateColor(const Vec4 & _Color)
{
	ID2D1SolidColorBrush* Color = FindColor(_Color);

	if (Color != nullptr)
	{
		return Color;
	}

	if (FAILED(GetDeviceInst->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_Color.r, _Color.g, _Color.b, _Color.a), &Color)))
	{
		return nullptr;
	}

	ColorMap.insert(std::unordered_map<unsigned int, ID2D1SolidColorBrush*>::value_type(CreateColorKey(_Color), Color));

	return Color;
}

ID2D1SolidColorBrush * FontManager::CreateColor(const int _Color)
{
	ID2D1SolidColorBrush* Color = FindColor(_Color);

	if (Color != nullptr)
	{
		return Color;
	}

	float r, g, b, a;

	b = (_Color & 0x000000ff) / 255.0f;
	g = ((_Color >> 8) & 0x000000ff) / 255.0f;
	r = ((_Color >> 16) & 0x000000ff) / 255.0f;
	a = ((_Color >> 24) & 0x000000ff) / 255.0f;

	if (FAILED(GetDeviceInst->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &Color)))
	{
		return nullptr;
	}

	ColorMap.insert(std::unordered_map<unsigned int, ID2D1SolidColorBrush*>::value_type(CreateColorKey(_Color), Color));

	return Color;
}

IDWriteTextLayout * FontManager::CreateTextLayout(const TCHAR * _Text, IDWriteTextFormat * _Format, float _Width, float _Height)
{
	IDWriteTextLayout* Layout = nullptr;

	if (FAILED(WriteFactory->CreateTextLayout(_Text, lstrlen(_Text), _Format, _Width, _Height, &Layout)))
	{
		return nullptr;
	}

	return Layout;
}

IDWriteTextLayout * FontManager::CreateTextLayout(const TCHAR * _Text, const std::string & _Font, float _Width, float _Height)
{
	IDWriteTextFormat* Format = FindTextFormat(_Font);

	if (Format == nullptr)
	{
		return nullptr;
	}

	IDWriteTextLayout* Layout = nullptr;

	if (FAILED(WriteFactory->CreateTextLayout(_Text, lstrlen(_Text), Format, _Width, _Height, &Layout)))
	{
		return nullptr;
	}

	return Layout;
}

IDWriteTextFormat * FontManager::FindTextFormat(const std::string & _Name)
{
	std::unordered_map<std::string, IDWriteTextFormat*>::iterator FindIter = FontMap.find(_Name);

	if (FindIter == FontMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

ID2D1SolidColorBrush * FontManager::FindColor(float _r, float _g, float _b, float _a)
{
	unsigned int Color = 0;

	unsigned char r, g, b, a;

	r = (unsigned char)(_r * 255);
	g = (unsigned char)(_g * 255);
	b = (unsigned char)(_b * 255);
	a = (unsigned char)(_a * 255);

	Color = a;
	Color = (Color << 8) | r;
	Color = (Color << 8) | g;
	Color = (Color << 8) | b;

	return FindColor(Color);
}

ID2D1SolidColorBrush * FontManager::FindColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	unsigned int Color = 0;

	Color = _a;
	Color = (Color << 8) | _r;
	Color = (Color << 8) | _g;
	Color = (Color << 8) | _b;

	return FindColor(Color);
}

ID2D1SolidColorBrush * FontManager::FindColor(const Vec4 & _Color)
{
	unsigned int Color = 0;

	unsigned char r, g, b, a;

	r = (unsigned char)(_Color.r * 255);
	g = (unsigned char)(_Color.g * 255);
	b = (unsigned char)(_Color.b * 255);
	a = (unsigned char)(_Color.a * 255);

	Color = a;
	Color = (Color << 8) | r;
	Color = (Color << 8) | g;
	Color = (Color << 8) | b;

	return FindColor(Color);
}

ID2D1SolidColorBrush * FontManager::FindColor(const int _Color)
{
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*>::iterator FindIter = ColorMap.find(_Color);

	if (FindIter == ColorMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

unsigned int FontManager::CreateColorKey(float _r, float _g, float _b, float _a)
{
	unsigned int Color = 0;

	unsigned char r, g, b, a;

	r = (unsigned char)(_r * 255);
	g = (unsigned char)(_g * 255);
	b = (unsigned char)(_b * 255);
	a = (unsigned char)(_a * 255);

	Color = a;
	Color = (Color << 8) | r;
	Color = (Color << 8) | g;
	Color = (Color << 8) | b;

	return Color;
}

unsigned int FontManager::CreateColorKey(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	unsigned int Color = 0;

	Color = _a;
	Color = (Color << 8) | _r;
	Color = (Color << 8) | _g;
	Color = (Color << 8) | _b;

	return Color;
}

unsigned int FontManager::CreateColorKey(const Vec4 & _Color)
{
	unsigned int Color = 0;

	unsigned char r, g, b, a;

	r = (unsigned char)(_Color.r * 255);
	g = (unsigned char)(_Color.g * 255);
	b = (unsigned char)(_Color.b * 255);
	a = (unsigned char)(_Color.a * 255);

	Color = a;
	Color = (Color << 8) | r;
	Color = (Color << 8) | g;
	Color = (Color << 8) | b;

	return Color;
}
