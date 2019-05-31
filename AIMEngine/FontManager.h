#pragma once

#include "EngineHeader.h"
class Engine_DLL FontManager
{
private:
	static IDWriteFactory* WriteFactory;
	static std::unordered_map<std::string, IDWriteTextFormat*> FontMap;
	static std::unordered_map<unsigned int, ID2D1SolidColorBrush*> ColorMap;

public:
	static bool Init();
	static void Release();
	static bool CreateTextFormat(const std::string& _Name, const TCHAR* _FontName, int _Weight, int _Style, int _Stretch, float _Size, const TCHAR* _LocalName);
	static ID2D1SolidColorBrush* CreateColor(float _r, float _g, float _b, float _a);
	static ID2D1SolidColorBrush* CreateColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);
	static ID2D1SolidColorBrush* CreateColor(const Vec4& _Color);
	static ID2D1SolidColorBrush* CreateColor(const int _Color);

	static IDWriteTextLayout* CreateTextLayout(const TCHAR* _Text, IDWriteTextFormat* _Format, float _Width, float _Height);
	static IDWriteTextLayout* CreateTextLayout(const TCHAR* _Text, const std::string& _Font, float _Width, float _Height);

	static IDWriteTextFormat* FindTextFormat(const std::string& _Name);
	static ID2D1SolidColorBrush* FindColor(float _r, float _g, float _b, float _a);
	static ID2D1SolidColorBrush* FindColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);
	static ID2D1SolidColorBrush* FindColor(const Vec4& _Color);
	static ID2D1SolidColorBrush* FindColor(const int _Color);

	static unsigned int CreateColorKey(float _r, float _g, float _b, float _a);
	static unsigned int CreateColorKey(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);
	static unsigned int CreateColorKey(const Vec4& _Color);
private:
	FontManager() {}
	virtual ~FontManager() = 0 {}
};

