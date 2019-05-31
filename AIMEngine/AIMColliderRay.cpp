#include "AIMColliderRay.h"
#include "InputManager.h"
#include "AIMDevice.h"
#include "AIMScene.h"
#include "AIMCamera.h"
#include "AIMColliderSphere.h"


AIMColliderRay::AIMColliderRay()
{
	SetNameTag("RayCol");
	ColType = CT_RAY;
}

AIMColliderRay::AIMColliderRay(const AIMColliderRay & _Other) : AIMCollider(_Other)
{
	Relative = _Other.Relative;
	Info = _Other.Info;
	Mouse = _Other.Mouse;

}


AIMColliderRay::~AIMColliderRay()
{
}

void AIMColliderRay::MouseEnable()
{
	Mouse = true;
}

void AIMColliderRay::Start()
{
	AIMCollider::Start();
}

bool AIMColliderRay::Init()
{
	return true;
}

int AIMColliderRay::Input(float _Time)
{
	return 0;
}

int AIMColliderRay::Update(float _Time)
{
	return 0;
}

int AIMColliderRay::LateUpdate(float _Time)
{
	if (Mouse == true)
	{
		Vec2 MousePos = InputManager::GetViewportMousePos();

		Resolution RS = GetDeviceInst->GetResolution();

		Ezptr<AIMCamera> Cam = Scene->GetMainCamera();

		Matrix Proj = Cam->GetProjection();
		Matrix View = Cam->GetView();

		View = View.Inverse();

		Info.Dir.x = (MousePos.x / (RS.Width / 2.0f) - 1.0f) / Proj._11;
		Info.Dir.y = (MousePos.y / (RS.Height / -2.0f) + 1.0f) / Proj._22;
		Info.Dir.z = 1.0f;

		Info.Origin = Vec3::Zero;

		Info.Dir = Info.Dir.TransformNormal(View);
		Info.Dir.Normalize();

		Info.Origin = Info.Origin.TransformCoord(View);

		Vec3 End = Info.Origin + Info.Dir * 1000.0f;

		SectionMin.x = Info.Origin.x < End.x ? Info.Origin.x : End.x;
		SectionMin.y = Info.Origin.y < End.y ? Info.Origin.y : End.y;
		SectionMin.z = Info.Origin.z < End.z ? Info.Origin.z : End.z;

		SectionMax.x = Info.Origin.x > End.x ? Info.Origin.x : End.x;
		SectionMax.y = Info.Origin.y > End.y ? Info.Origin.y : End.y;
		SectionMax.z = Info.Origin.z > End.z ? Info.Origin.z : End.z;
	}
	else
	{

	}

	return 0;
}

int AIMColliderRay::Collision(float _Time)
{
	return 0;
}

int AIMColliderRay::PrevRender(float _Time)
{
	return 0;
}

int AIMColliderRay::Render(float _Time)
{
	return 0;
}

AIMColliderRay * AIMColliderRay::Clone() const
{
	return new AIMColliderRay(*this);
}

bool AIMColliderRay::Collision(Ezptr<AIMCollider> _Dest)
{
	switch (_Dest->GetColType())
	{
	case CT_SPHERE:
		return CollisionSp2Ray(((Ezptr<AIMColliderSphere>)_Dest)->GetInfo(), Info);
	case CT_CAPSULE:
		return false;
	case CT_OBB:
		return false;
	case CT_POINT:
		return false;
	case CT_RAY:
		return false;
	case CT_AABB:
		return false;

	}

	return false;
}
