#include "AIMNavigation.h"
#include "AIMTransform.h"
#include "NavigationManager.h"
#include "NavigationMesh.h"
#include "AIMObject.h"


AIMNavigation::AIMNavigation()
{
	CT = CT_NAVIGATION;
}

AIMNavigation::AIMNavigation(const AIMNavigation & _Other) : AIMComponent(_Other)
{
	OnCellYPosition = _Other.OnCellYPosition;
	MouseMove = _Other.MouseMove;
	Find = _Other.Find;
	PathList = _Other.PathList;
	Target = _Other.Target;
	Way = _Other.Way;
	TargetPos = _Other.TargetPos;
	TargetDetectTime = _Other.TargetDetectTime;
}


AIMNavigation::~AIMNavigation()
{
}

void AIMNavigation::SetCellYPosition(bool _Enable)
{
	OnCellYPosition = true;
}

void AIMNavigation::SetMouseMove(bool _Enable)
{
	MouseMove = true;
}

void AIMNavigation::SetAIFindPath(bool _Enable)
{
	Find = _Enable;

	if (_Enable == false)
	{
		PathList.clear();
		TargetDetectTime = 0.0f;
	}
	else if (Target)
	{
		TargetPos = Target->GetWorldPosition();
	}
}

void AIMNavigation::SetTarget(Ezptr<AIMObject> _Target)
{
	if (_Target != nullptr)
	{
		Target = _Target->GetTransform();
		TargetPos = Target->GetWorldPosition();
	}
	else
	{
		_Target = nullptr;
		TargetPos = Vec3::Zero;
	}

	TargetDetectTime = 0.0f;
}

void AIMNavigation::Start()
{
}

bool AIMNavigation::Init()
{
	return true;
}

int AIMNavigation::Input(float _Time)
{
	return 0;
}

int AIMNavigation::Update(float _Time)
{
	if (Find == true)
	{
		if (Target != nullptr)
		{
			TargetDetectTime += _Time;

			if (TargetDetectTime >= 1.0f)
			{
				TargetDetectTime -= 1.0f;
				Vec3 TmpTargetPos = Target->GetWorldPosition();

				if (TargetPos != TmpTargetPos)
				{
					TargetPos = TmpTargetPos;
					PathList.clear();
					Way = Vec3::Zero;
				}
			}
		}

		if (PathList.empty() == true)
		{
			Ezptr<NavigationMesh> NaviMesh = NavigationManager::FindNaviMesh(Scene, Transform->GetWorldPosition());

			Vec3 Tmp = Transform->GetWorldPosition();

			NaviMesh->FindPath(Transform->GetWorldPosition(), Target->GetWorldPosition());

			PathList = NaviMesh->GetPathList();

			Way = PathList.front();
			PathList.pop_front();
		}


		Vec3 Dir = Way - Transform->GetWorldPosition();
		Dir.Normalize();

		float Dist = Way.Distance(Transform->GetWorldPosition());

		float Speed = 3.0f * _Time;

		if (Dist < Speed)
		{
			Speed = Dist;

			if (PathList.empty() == false)
			{
				Way = PathList.front();
				PathList.pop_front();
			}
			else
			{
				Way = Vec3::Zero;
				Find = false;
			}
		}

		Transform->Move(Dir, 3.0f, _Time);
	}

	return 0;
}

int AIMNavigation::LateUpdate(float _Time)
{
	if (OnCellYPosition == true)
	{
		Vec3 Pos = Transform->GetWorldPosition();
		Ezptr<NavigationMesh> NaviMesh = NavigationManager::FindNaviMesh(Scene, Pos);

		if (NaviMesh != nullptr)
		{
			if (NaviMesh->GetCellEnable(Pos) == true)
			{
				Pos.y = NaviMesh->GetY(Pos);

				Transform->SetWorldPosition(Pos);
			}
			else if (MouseMove == false && Find == false)
			{
				Transform->MoveBack();
			}
		}
	}

	return 0;
}

int AIMNavigation::Collision(float _Time)
{
	return 0;
}

int AIMNavigation::PrevRender(float _Time)
{
	return 0;
}

int AIMNavigation::Render(float _Time)
{
	return 0;
}

AIMNavigation * AIMNavigation::Clone() const
{
	return new AIMNavigation(*this);
}
