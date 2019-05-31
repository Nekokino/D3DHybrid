#include "AIMArm.h"
#include "AIMObject.h"
#include "AIMTransform.h"
#include "InputManager.h"


AIMArm::AIMArm()
{
	CT = CT_ARM;
}

AIMArm::AIMArm(const AIMArm & _Other) : AIMComponent(_Other)
{
	Target = nullptr;
	MouseEnable = _Other.MouseEnable;
	Dist = 0.0f;
}


AIMArm::~AIMArm()
{
}

void AIMArm::SetTarget(Ezptr<AIMObject> _Target)
{
	Target = _Target->GetTransform();

	if (Dist.Length() == 0.0f)
	{
		Dist = Target->GetWorldPosition() - Transform->GetWorldPosition();
	}
}

void AIMArm::SetTarget(Ezptr<AIMComponent> _Target)
{
	Target = _Target->GetTransform();

	if (Dist.Length() == 0.0f)
	{
		Dist = Target->GetWorldPosition() - Transform->GetWorldPosition();
	}
}

void AIMArm::SetDist(const Vec3 & _Dist)
{
	Dist = _Dist;
}

void AIMArm::SetMouseEnable(bool _Enable)
{
	MouseEnable = _Enable;
}

void AIMArm::Start()
{
}

bool AIMArm::Init()
{
	return true;
}

int AIMArm::Input(float _Time)
{
	return 0;
}

int AIMArm::Update(float _Time)
{
	if (MouseEnable == true)
	{
		short Wheel = InputManager::GetWheelDir();

		if (Wheel != 0)
		{
			Transform->Move(AXIS_Z, 20.0f * Wheel, _Time);
		}

		if (InputManager::MousePush(MB_RBUTTON) == true)
		{
			Vec2 Move = InputManager::GetViewportMouseMove();

			if (Move.x != 0.0f)
			{
				Transform->RotationY(Move.x);
			}

			if (Move.y != 0.0f)
			{
				Transform->RotationX(Move.y);
			}

			float Dist = Transform->GetWorldPosition().Distance(Target->GetWorldPosition());

			Transform->SetWorldPosition(Target->GetWorldPosition() + Transform->GetWorldAxis(AXIS_Z) * -Dist);
		}
	}

	return 0;
}

int AIMArm::LateUpdate(float _Time)
{
	if (Target != nullptr)
	{
		Vec3 Move = Target->GetMove();

		if (Move.Length() != 0.0f)
		{
			Transform->Move(Move);
		}
	}

	return 0;
}

int AIMArm::Collision(float _Time)
{
	return 0;
}

int AIMArm::PrevRender(float _Time)
{
	return 0;
}

int AIMArm::Render(float _Time)
{
	return 0;
}

AIMArm * AIMArm::Clone() const
{
	return new AIMArm(*this);
}
