#pragma once

#include "AIMComponent.h"

enum TextAlignH
{
	TAH_LEFT,
	TAH_CENTER,
	TAH_RIGHT,
};

enum TextAlignV
{
	TAV_TOP,
	TAV_MID,
	TAV_BOTTOM,
};

enum TextRenderType
{
	TRT_UI,
	TRT_2D,
	TRT_3D,
};

class Engine_DLL AIMText : public AIMComponent
{
	friend class AIMObject;
private:
	IDWriteTextFormat* TextFormat = nullptr;
	IDWriteTextLayout* TextLayout = nullptr;

	ID2D1SolidColorBrush* Color = nullptr;
	ID2D1SolidColorBrush* ShadowColor = nullptr;

	Vec3 Offset;

	ID2D1RenderTarget* _2DTarget;

	bool Shadow;
	TCHAR* Text = nullptr;
	int MaxCount = 0;
	TCHAR* Font = nullptr;
	float Size = 0.0f;
	TextAlignH AlignH;
	TextAlignV AlignV;
	TextRenderType RenderType;

	bool Alpha = true;
	bool AlphaShadow = true;

	float Opacity;
	float ShadowOpacity;

	Vec4 ColorVec;
	Vec4 ShadowColorVec;
	Vec3 ShadowOffset;
	D2D1_RECT_F RenderArea;

public:
	void SetRenderType(TextRenderType _Type);
	void SetTextCount(int _MaxCount);
	void SetText(const TCHAR* _Text);
	void AddText(const TCHAR* _Text);

	void SetFont(const std::string& _Name, const TCHAR* _FontName, int _Weight, int _Style, int _Stretch, float _Size, const TCHAR* _LocalName);

	void SetFont(const std::string& _Name);
	void SetSize(float _Size);
	void SetAlignH(TextAlignH _Align);
	void SetAlignV(TextAlignV _Align);
	void SetShadow(bool _Shadow);
	void SetOffset(const Vec3& _Offset);
	void SetShadowOffset(const Vec3& _ShadowOffset);
	void SetShadowColor(float _r, float _g, float _b, float _a);
	void SetShadowColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);
	void SetShadowColor(const Vec4& _Color);
	void SetShadowColor(unsigned int _Color);
	void AlphaBlend(bool _Alpha);
	void ShadowAlphaBlend(bool _Alpha);
	void SetOpacity(float _Opacity);
	void SetShadowOpacity(float _Opacity);
	void SetColor(float _r, float _g, float _b, float _a);
	void SetColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);
	void SetColor(const Vec4& _Color);
	void SetColor(unsigned int _Color);
	void SetRenderArea(float _l, float _t, float _r, float _b);

private:
	void CreateTextLayout();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMText* Clone() const;


protected:
	AIMText();
	AIMText(const AIMText& _Other);
	~AIMText();
};

