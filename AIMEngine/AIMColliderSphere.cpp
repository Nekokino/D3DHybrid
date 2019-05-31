#include "AIMColliderSphere.h"

#ifdef _DEBUG
#include "AIMMesh.h"
#include "ResourceManager.h"
#include "AIMScene.h"
#include "AIMCamera.h"
#endif

#include "AIMTransform.h"
#include "AIMColliderRay.h"


AIMColliderSphere::AIMColliderSphere()
{
	SetNameTag("SphereCol");
	ColType = CT_SPHERE;
#ifdef _DEBUG
	Mesh = ResourceManager::FindMesh("Sky");
#endif
}

AIMColliderSphere::AIMColliderSphere(const AIMColliderSphere & _Other) : AIMCollider(_Other)
{
	Relative = _Other.Relative;
	Info = _Other.Info;
	ScaleEnable = _Other.ScaleEnable;

#ifdef _DEBUG
	Mesh = _Other.Mesh;
#endif
}


AIMColliderSphere::~AIMColliderSphere()
{
#ifdef _DEBUG
	Mesh = nullptr;
#endif
}

void AIMColliderSphere::SetSphereInfo(const Vec3 & _Center, float _Radius)
{
	Relative.Center = _Center;
	Relative.Radius = _Radius;
	Info.Radius = _Radius;
}

void AIMColliderSphere::SetScaleEnable(bool _Enable)
{
	ScaleEnable = _Enable;
}

SphereInfo AIMColliderSphere::GetInfo() const
{
	return Info;
}

void AIMColliderSphere::Start()
{
	AIMCollider::Start();
}

bool AIMColliderSphere::Init()
{
	return true;
}

int AIMColliderSphere::Input(float _Time)
{
	return 0;
}

int AIMColliderSphere::Update(float _Time)
{
	return 0;
}

int AIMColliderSphere::LateUpdate(float _Time)
{
	if (ScaleEnable == true)
	{
		Vec3 Scale = Transform->GetWorldScale();

		Info.Center = Relative.Center * Scale + Transform->GetWorldPosition();

		float fScale = Scale.x;

		fScale = fScale < Scale.y ? Scale.y : fScale;
		fScale = fScale < Scale.z ? Scale.z : fScale;

		Info.Radius = Relative.Radius * fScale;
	}
	else
	{
		Info.Center = Relative.Center + Transform->GetWorldPosition();
		Info.Radius = Relative.Radius;
	}
	
	SectionMin = Info.Center - Info.Radius;
	SectionMax = Info.Center + Info.Radius;

	return 0;
}

int AIMColliderSphere::Collision(float _Time)
{
	return 0;
}

int AIMColliderSphere::PrevRender(float _Time)
{
	return 0;
}

int AIMColliderSphere::Render(float _Time)
{
#ifdef _DEBUG
	Ezptr<AIMCamera> Cam = Scene->GetMainCamera();

	Matrix ScaleMat, TransMat, WorldMat;

	ScaleMat.Scaling(Info.Radius, Info.Radius, Info.Radius);
	TransMat.Translation(Info.Center);

	WorldMat = ScaleMat * TransMat;

	CBuffer.WorldRotation.Identify();
	CBuffer.WVRotation = CBuffer.WorldRotation * Cam->GetView();
	CBuffer.World = WorldMat;
	CBuffer.View = Cam->GetView();
	CBuffer.Projection = Cam->GetProjection();
	CBuffer.InvProjection = CBuffer.Projection;
	CBuffer.InvProjection.Inverse();
	CBuffer.WV = CBuffer.World * CBuffer.View;
	CBuffer.WVP = CBuffer.WV * CBuffer.Projection;

	CBuffer.WorldRotation.Transpose();
	CBuffer.WVRotation.Transpose();
	CBuffer.World.Transpose();
	CBuffer.View.Transpose();
	CBuffer.Projection.Transpose();
	CBuffer.InvProjection.Transpose();
	CBuffer.WV.Transpose();
	CBuffer.WVP.Transpose();
#endif

	AIMCollider::Render(_Time);

	return 0;
}

AIMColliderSphere * AIMColliderSphere::Clone() const
{
	return new AIMColliderSphere(*this);
}

bool AIMColliderSphere::Collision(Ezptr<AIMCollider> _Dest)
{
	switch (_Dest->GetColType())
	{
	case CT_SPHERE:
		return CollisionSp2Sp(Info, ((Ezptr<AIMColliderSphere>)_Dest)->GetInfo());
		break;
	case CT_CAPSULE:
		return false;
	case CT_OBB:
		return false;
	case CT_POINT:
		return false;
	case CT_RAY:
		return CollisionSp2Ray(Info, ((Ezptr<AIMColliderRay>)_Dest)->GetInfo());
	case CT_AABB:
		return false;

	}

	return false;
}
