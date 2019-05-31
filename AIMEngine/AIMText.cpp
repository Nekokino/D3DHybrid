#include "AIMText.h"
#include "AIMDevice.h"
#include "FontManager.h"
#include "AIMScene.h"
#include "AIMCamera.h"
#include "AIMTransform.h"
#include "AIMObject.h"


AIMText::AIMText() : Offset(0.0f, 0.0f, 0.0f), _2DTarget(GetDeviceInst->Get2DRenderTarget()), Shadow(false)
{
	CT = CT_TEXT;
	SetNameTag("Text");

	Font = new TCHAR[64];
	memset(Font, 0, sizeof(TCHAR) * 64);
	lstrcpy(Font, TEXT("±Ã¼­Ã¼"));

	Size = 20.0f;
	AlignH = TAH_LEFT;
	AlignV = TAV_MID;
	RenderType = TRT_UI;
	Alpha = false;
	AlphaShadow = false;
	Opacity = 1.0f;
	ShadowOpacity = 1.0f;
	ColorVec = Vec4::White;
	ShadowColorVec = Vec4::Black;
	TextFormat = FontManager::FindTextFormat("±Ã¼­Ã¼");
	TextLayout = FontManager::CreateTextLayout(TEXT(""), TextFormat, 100.0f, 100.0f);

	Color = FontManager::CreateColor(ColorVec);
	ShadowColor = FontManager::CreateColor(ShadowColorVec);

	MaxCount = 256;
	Text = new TCHAR[MaxCount];

	memset(Text, 0, sizeof(TCHAR) * MaxCount);

	lstrcpy(Text, TEXT("Text"));

	RenderArea = {};
}

AIMText::AIMText(const AIMText & _Other) : AIMComponent(_Other)
{
	TextFormat = _Other.TextFormat;
	Color = _Other.Color;
	ShadowColor = _Other.ShadowColor;
	Offset = _Other.Offset;
	_2DTarget = _Other._2DTarget;

	Shadow = _Other.Shadow;
	MaxCount = _Other.MaxCount;
	Size = _Other.Size;
	AlignH = _Other.AlignH;
	AlignV = _Other.AlignV;
	RenderType = _Other.RenderType;

	Alpha = _Other.Alpha;
	AlphaShadow = _Other.AlphaShadow;

	Opacity = _Other.Opacity;
	ShadowOpacity = _Other.ShadowOpacity;
	
	ColorVec = _Other.ColorVec;
	ShadowColorVec = _Other.ShadowColorVec;
	ShadowOffset = _Other.ShadowOffset;

	Text = new TCHAR[MaxCount];
	memcpy(Text, _Other.Text, sizeof(TCHAR) * MaxCount);

	Font = new TCHAR[64];
	memset(Font, 0, sizeof(TCHAR) * 64);
	lstrcpy(Font, _Other.Font);
	TextLayout = nullptr;
	CreateTextLayout();

	RenderArea = {};
}


AIMText::~AIMText()
{
	if (TextLayout != nullptr)
	{
		TextLayout->Release();
		TextLayout = nullptr;
	}

	if (Text != nullptr)
	{
		delete[] Text;
		Text = nullptr;
	}

	if (Font != nullptr)
	{
		delete[] Font;
		Font = nullptr;
	}
}

void AIMText::SetRenderType(TextRenderType _Type)
{
	RenderType = _Type;
}

void AIMText::SetTextCount(int _MaxCount)
{
	if (MaxCount >= _MaxCount)
	{
		return;
	}

	TCHAR* NewText = new TCHAR[_MaxCount];

	memset(NewText, 0, sizeof(TCHAR) * _MaxCount);
	memcpy(NewText, Text, MaxCount);

	MaxCount = _MaxCount;

	delete[] Text;

	Text = NewText;
}

void AIMText::SetText(const TCHAR * _Text)
{
	int Count = lstrlen(_Text);

	if (MaxCount < Count)
	{
		MaxCount = Count;

		delete[] Text;

		Text = new TCHAR[MaxCount];
	}

	memset(Text, 0, sizeof(TCHAR) * MaxCount);
	lstrcpy(Text, _Text);
}

void AIMText::AddText(const TCHAR * _Text)
{
	int Count = lstrlen(_Text);
	int CurCount = lstrlen(Text);

	if (MaxCount < Count + CurCount)
	{
		int NewMaxCount = (Count + CurCount) * 2;

		TCHAR* NewText = new TCHAR[NewMaxCount];
		memset(NewText, 0, sizeof(TCHAR) * NewMaxCount);

		lstrcpy(NewText, Text);

		delete[] Text;

		Text = NewText;

		MaxCount = NewMaxCount;
	}

	lstrcat(Text, _Text);
}

void AIMText::SetFont(const std::string & _Name, const TCHAR * _FontName, int _Weight, int _Style, int _Stretch, float _Size, const TCHAR * _LocalName)
{
	lstrcpy(Font, _FontName);
	Size = _Size;

	FontManager::CreateTextFormat(_Name, _FontName, _Weight, _Style, _Stretch, _Size, _LocalName);

	TextFormat = FontManager::FindTextFormat(_Name);

	CreateTextLayout();
}

void AIMText::SetFont(const std::string & _Name)
{
	TextFormat = FontManager::FindTextFormat(_Name);

	CreateTextLayout();
}

void AIMText::SetSize(float _Size)
{
	Size = _Size;
	
	CreateTextLayout();
}

void AIMText::SetAlignH(TextAlignH _Align)
{
	AlignH = _Align;

	if (TextLayout == nullptr)
	{
		CreateTextLayout();
	}

	switch (_Align)
	{
	case TAH_LEFT:
		TextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case TAH_CENTER:
		TextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case TAH_RIGHT:
		TextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	default:
		break;
	}
}

void AIMText::SetAlignV(TextAlignV _Align)
{
	AlignV = _Align;

	if (TextLayout == nullptr)
	{
		CreateTextLayout();
	}
	
	switch (_Align)
	{
	case TAV_TOP:
		TextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case TAV_MID:
		TextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case TAV_BOTTOM:
		TextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	default:
		break;
	}
}

void AIMText::SetShadow(bool _Shadow)
{
	Shadow = _Shadow;
}

void AIMText::SetOffset(const Vec3 & _Offset)
{
	Offset = _Offset;
}

void AIMText::SetShadowOffset(const Vec3 & _ShadowOffset)
{
	ShadowOffset = _ShadowOffset;
}

void AIMText::SetShadowColor(float _r, float _g, float _b, float _a)
{
	ShadowColorVec = Vec4(_r, _g, _b, _a);

	ShadowColor = FontManager::CreateColor(_r, _g, _b, _a);
}

void AIMText::SetShadowColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	ShadowColorVec = Vec4(_r / 255.0f, _g / 255.0f, _b / 255.0f, _a / 255.0f);

	ShadowColor = FontManager::CreateColor(_r, _g, _b, _a);
}

void AIMText::SetShadowColor(const Vec4 & _Color)
{
	ShadowColorVec = _Color;

	ShadowColor = FontManager::CreateColor(_Color);
}

void AIMText::SetShadowColor(unsigned int _Color)
{
	ShadowColorVec.b = (_Color & 0x000000ff) / 255.0f;
	ShadowColorVec.g = ((_Color << 8) & 0x000000ff) / 255.0f;
	ShadowColorVec.r = ((_Color << 16) & 0x000000ff) / 255.0f;
	ShadowColorVec.a = ((_Color << 24) & 0x000000ff) / 255.0f;

	ShadowColor = FontManager::CreateColor(_Color);
}

void AIMText::AlphaBlend(bool _Alpha)
{
	Alpha = _Alpha;
}

void AIMText::ShadowAlphaBlend(bool _Alpha)
{
	AlphaShadow = _Alpha;
}

void AIMText::SetOpacity(float _Opacity)
{
	Opacity = _Opacity;
}

void AIMText::SetShadowOpacity(float _Opacity)
{
	ShadowOpacity = _Opacity;
}

void AIMText::SetColor(float _r, float _g, float _b, float _a)
{
	ColorVec = Vec4(_r, _g, _b, _a);

	Color = FontManager::CreateColor(_r, _g, _b, _a);
}

void AIMText::SetColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	ColorVec = Vec4(_r / 255.0f, _g / 255.0f, _b / 255.0f, _a / 255.0f);

	Color = FontManager::CreateColor(_r, _g, _b, _a);
}

void AIMText::SetColor(const Vec4 & _Color)
{
	ColorVec = _Color;

	Color = FontManager::CreateColor(_Color);
}

void AIMText::SetColor(unsigned int _Color)
{
	ColorVec.b = (_Color & 0x000000ff) / 255.0f;
	ColorVec.g = ((_Color << 8) & 0x000000ff) / 255.0f;
	ColorVec.r = ((_Color << 16) & 0x000000ff) / 255.0f;
	ColorVec.a = ((_Color << 24) & 0x000000ff) / 255.0f;

	Color = FontManager::CreateColor(_Color);
}

void AIMText::SetRenderArea(float _l, float _t, float _r, float _b)
{
	RenderArea = D2D1::RectF(_l, GetDeviceInst->GetResolution().Height - _b, _r, GetDeviceInst->GetResolution().Height - _t);

	CreateTextLayout();
}

void AIMText::CreateTextLayout()
{
	if (TextFormat == nullptr)
	{
		return;
	}

	if (TextLayout != nullptr)
	{
		TextLayout->Release();
		TextLayout = nullptr;
	}

	TextLayout = FontManager::CreateTextLayout(Text, TextFormat, RenderArea.right - RenderArea.left, RenderArea.bottom - RenderArea.top);

	DWRITE_TEXT_RANGE Range;
	Range.startPosition = 0;
	Range.length = lstrlen(Text);

	TextLayout->SetFontSize(Size, Range);

	
}

void AIMText::Start()
{
	if (RenderType == TRT_UI)
	{
		Object->SetRenderGroup(RG_UI);
	}
}

bool AIMText::Init()
{
	return true;
}

int AIMText::Input(float _Time)
{
	return 0;
}

int AIMText::Update(float _Time)
{
	return 0;
}

int AIMText::LateUpdate(float _Time)
{
	return 0;
}

int AIMText::Collision(float _Time)
{
	return 0;
}

int AIMText::PrevRender(float _Time)
{
	return 0;
}

int AIMText::Render(float _Time)
{
	_2DTarget->BeginDraw();

	Vec3 Pos = Transform->GetWorldPosition();

	float Height = RenderArea.bottom - RenderArea.top;

	if (RenderType != TRT_UI)
	{
		Ezptr<AIMTransform> CamTr = Scene->GetMainCameraTransform();

		Pos -= CamTr->GetWorldPosition();
	}

	if (Shadow == true)
	{
		Vec3 ShadowPos = Pos + ShadowOffset;

		if (AlphaShadow == true)
		{
			ShadowColor->SetOpacity(ShadowOpacity);
		}
		else
		{
			ShadowColor->SetOpacity(1.0f);
		}

		ShadowPos.y = GetDeviceInst->GetResolution().Height - ShadowPos.y - Height;

		_2DTarget->DrawTextLayout(D2D1::Point2F(ShadowPos.x, ShadowPos.y), TextLayout, ShadowColor);
	}

	if (Alpha == true)
	{
		Color->SetOpacity(Opacity);
	}
	else
	{
		Color->SetOpacity(1.0f);
	}

	Pos.y = GetDeviceInst->GetResolution().Height - Pos.y - Height;

	_2DTarget->DrawTextLayout(D2D1::Point2F(Pos.x, Pos.y), TextLayout, Color);

	_2DTarget->EndDraw();

	return 0;
}

AIMText * AIMText::Clone() const
{
	return new AIMText(*this);
}
