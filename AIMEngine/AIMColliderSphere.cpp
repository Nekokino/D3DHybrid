#include "AIMColliderSphere.h"

#ifdef _DEBUG
#include "AIMMesh.h"
#include "ResourceManager.h"
#include "AIMScene.h"
#include "AIMCamera.h"
#endif

#include "AIMTransform.h"


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

#ifdef _DEBUG
	Mesh = _Other.Mesh;
#endif
}


AIMColliderSphere::~AIMColliderSphere()
{
}

void AIMColliderSphere::SetSphereInfo(const Vec3 & _Center, float _Radius)
{
	Relative.Center = _Center;
	Relative.Radius = _Radius;
	Info.Radius = _Radius;
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
	Info.Center = Relative.Center + Transform->GetWorldPosition();

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
		return false;
	case CT_AABB:
		return false;

	}

	return false;
}
