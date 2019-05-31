#include "AIMUIButton.h"
#include "AIMRenderer.h"
#include "AIMMaterial.h"
#include "AIMTransform.h"
#include "AIMObject.h"
#include "AIMTexture.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "InputManager.h"


AIMUIButton::AIMUIButton()
{
	Color[BS_DISABLE] = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
	Color[BS_NORMAL] = Vec4(0.8f, 0.8f, 0.8f, 1.0f);
	Color[BS_MOUSEON] = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Color[BS_CLICK] = Vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

AIMUIButton::AIMUIButton(const AIMUIButton & _Other) : AIMUI(_Other)
{
	State = _Other.State;
	PrevState = _Other.State;

	for (int i = 0; i < BS_END; i++)
	{
		CallBack[i] = nullptr;
		Texture[i] = _Other.Texture[i];
		Color[i] = _Other.Color[i];
	}
}


AIMUIButton::~AIMUIButton()
{
}

void AIMUIButton::SetStateColor(ButtonState _State, const Vec4 & _Color)
{
	Color[_State] = _Color;
}

void AIMUIButton::SetStateTexture(ButtonState _State, const std::string & _Name, const TCHAR * _FileName, const std::string & _Path)
{
	if (ResourceManager::LoadTexture(_Name, _FileName, _Path) == false)
	{
		return;
	}

	Texture[_State] = ResourceManager::FindTexture(_Name);

	if (_State == State && Material != nullptr)
	{
		Material->DeleteTextureSet(0, 0, 0);

		Material->AddTextureSet(0, 0, 0, Texture[_State]);
	}
}

void AIMUIButton::SetCallBack(ButtonState _State, void(*_Func)(float))
{
}

void AIMUIButton::Start()
{
	Material = FindComponent<AIMMaterial>(CT_MATERIAL);

	if (Texture[State] != nullptr)
	{
		//Material->DeleteTextureSet(0, 0, 0);
		// ????

		Material->AddTextureSet(0, 0, 0, Texture[State]);
	}
}

bool AIMUIButton::Init()
{
	if (AIMUI::Init() == false)
	{
		return false;
	}

	Transform->SetWorldScale(200.0f, 100.0f, 1.0f);

	Ezptr<AIMRenderer> Renderer = AddComponent<AIMRenderer>("ButtonRenderer");

	Renderer->SetMesh("Rect");
	Renderer->SetShader("ButtonShader");

	Renderer->SetRenderState("AlphaBlend");

	Ezptr<AIMMaterial> Material = AddComponent<AIMMaterial>("ButtonMaterial");

	Material->AddTextureSet(0, 0, 0, "ButtonDefaultNormal", TEXT("UIDefault.png"));

	StartTextureChange = true;

	return true;
}

int AIMUIButton::Input(float _Time)
{
	return 0;
}

int AIMUIButton::Update(float _Time)
{
	if (State != BS_DISABLE)
	{
		PrevState = State;
		
		Vec2 MousePos = InputManager::GetViewportMousePos();

		Vec3 Pos = Transform->GetWorldPosition();
		Vec3 Scale = Transform->GetWorldScale();

		if (Pos.x <= MousePos.x && MousePos.x <= Pos.x + Scale.x && Pos.y <= MousePos.y && MousePos.y <= Pos.y + Scale.y)
		{
			if (InputManager::MousePress(MB_LBUTTON) == true)
			{
				State = BS_CLICK;
			}
			else if (InputManager::MousePush(MB_LBUTTON) == true)
			{
				if (State != BS_CLICK)
				{
					State = BS_MOUSEON;
				}
			}
			else if (InputManager::MouseRelease(MB_LBUTTON) && State == BS_CLICK)
			{
				CallBack[BS_CLICK](_Time);
			}
			else
			{
				State = BS_MOUSEON;
			}
		}
		else
		{
			State = BS_NORMAL;
		}
	}

	if (CallBack[State] != nullptr && State != BS_CLICK)
	{
		CallBack[State](_Time);
	}

	return 0;
}

int AIMUIButton::LateUpdate(float _Time)
{
	return 0;
}

int AIMUIButton::Collision(float _Time)
{
	return 0;
}

int AIMUIButton::PrevRender(float _Time)
{
	CBuffer.Color = Color[State];

	ShaderManager::UpdateConstBuffer("Button", &CBuffer);

	if (PrevState != State && Texture[State] != nullptr)
	{
		//Material->DeleteTextureSet(0, 0, 0);
		// ??????

		Material->AddTextureSet(0, 0, 0, Texture[State]);
	}

	return 0;
}

int AIMUIButton::Render(float _Time)
{
	return 0;
}

AIMUIButton * AIMUIButton::Clone() const
{
	return new AIMUIButton(*this);
}
