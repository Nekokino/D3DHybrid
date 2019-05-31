#include "AIMColliderOBB.h"

#ifdef _DEBUG
#include "AIMMesh.h"
#include "ResourceManager.h"
#include "AIMScene.h"
#include "AIMCamera.h"
#endif

#include "AIMTransform.h"



AIMColliderOBB::AIMColliderOBB()
{
	SetNameTag("OBBCol");
	ColType = CT_OBB;

#ifdef _DEBUG
	Mesh = ResourceManager::FindMesh("ColliderBox");
#endif
}

AIMColliderOBB::AIMColliderOBB(const AIMColliderOBB & _Other) : AIMCollider(_Other)
{
	Relative = _Other.Relative;
	Info = _Other.Info;
}


AIMColliderOBB::~AIMColliderOBB()
{
}

void AIMColliderOBB::SetOBBInfo(const Vec3 & _Center, const Vec3 & _HalfLength)
{
	Relative.Center = _Center;
	Relative.HalfLength = _HalfLength;
	Info.HalfLength = _HalfLength;
}

void AIMColliderOBB::Start()
{
	AIMCollider::Start();
}

bool AIMColliderOBB::Init()
{
	return true;
}

int AIMColliderOBB::Input(float _Time)
{
	return 0;
}

int AIMColliderOBB::Update(float _Time)
{
	return 0;
}

int AIMColliderOBB::LateUpdate(float _Time)
{
	Info.Center = Relative.Center + Transform->GetWorldPosition();

	Matrix WorldRotMat = Transform->GetWorldRotationMatrix();

	for (int i = 0; i < AXIS_END; i++)
	{
		Info.Axis[i] = Vec3::Axis[i].TransformNormal(WorldRotMat);
		Info.Axis[i].Normalize();
	}

	Vec3 Pos[8];

	Pos[0] = Vec3(-Info.HalfLength.x, Info.HalfLength.y, -Info.HalfLength.z);
	Pos[1] = Vec3(Info.HalfLength.x, Info.HalfLength.y, -Info.HalfLength.z);
	Pos[2] = Vec3(-Info.HalfLength.x, -Info.HalfLength.y, -Info.HalfLength.z);
	Pos[3] = Vec3(Info.HalfLength.x, -Info.HalfLength.y, -Info.HalfLength.z);
	Pos[4] = Vec3(-Info.HalfLength.x, Info.HalfLength.y, Info.HalfLength.z);
	Pos[5] = Vec3(Info.HalfLength.x, Info.HalfLength.y, Info.HalfLength.z);
	Pos[6] = Vec3(-Info.HalfLength.x, -Info.HalfLength.y, Info.HalfLength.z);
	Pos[7] = Vec3(Info.HalfLength.x, -Info.HalfLength.y, Info.HalfLength.z);

	for (int i = 0; i < 8; i++)
	{
		Pos[i] = Pos[i].TransformCoord(WorldRotMat);
		Pos[i] += Info.Center;
	}

	SectionMin = Vec3(100000.0f, 100000.0f, 100000.0f);
	SectionMax = Vec3(-100000.0f, -100000.0f, -100000.0f);

	for (int i = 0; i < 8; i++)
	{
		if (SectionMin.x > Pos[i].x)
		{
			SectionMin.x = Pos[i].x;
		}

		if (SectionMin.y > Pos[i].y)
		{
			SectionMin.y = Pos[i].y;
		}

		if (SectionMin.z > Pos[i].z)
		{
			SectionMin.z = Pos[i].z;
		}

		if (SectionMax.x < Pos[i].x)
		{
			SectionMax.x = Pos[i].x;
		}

		if (SectionMax.y < Pos[i].y)
		{
			SectionMax.y = Pos[i].y;
		}

		if (SectionMax.z < Pos[i].z)
		{
			SectionMax.z = Pos[i].z;
		}
	}

	return 0;
}

int AIMColliderOBB::Collision(float _Time)
{
	return 0;
}

int AIMColliderOBB::PrevRender(float _Time)
{
	return 0;
}

int AIMColliderOBB::Render(float _Time)
{
#ifdef _DEBUG
	Ezptr<AIMCamera> Cam = Scene->GetMainCamera();

	Matrix ScaleMat, TransMat, WorldMat, RotationMat;

	for (int i = 0; i < AXIS_END; i++)
	{
		memcpy(&RotationMat[i][0], &Info.Axis[i], sizeof(Vec3));
	}

	ScaleMat.Scaling(Info.HalfLength);
	TransMat.Translation(Info.Center);

	WorldMat = ScaleMat * RotationMat * TransMat;

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

AIMColliderOBB * AIMColliderOBB::Clone() const
{
	return new AIMColliderOBB(*this);
}

bool AIMColliderOBB::Collision(Ezptr<AIMCollider> _Dest)
{
	switch (_Dest->GetColType())
	{
	case CT_SPHERE:
		return false;
	case CT_CAPSULE:
		return false;
	case CT_OBB:
		return CollisionOBB2OBB(Info, ((Ezptr<AIMColliderOBB>)_Dest)->GetInfo());
	case CT_POINT:
		return false;
	case CT_RAY:
	{
		int a = 0;
	}
		return false;
	case CT_AABB:
		return false;
	}

	return false;
}
