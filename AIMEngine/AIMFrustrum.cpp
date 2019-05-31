#include "AIMFrustrum.h"

AIMFrustrum::AIMFrustrum()
{
	Pos[0] = Vec3(-1.0f, 1.0f, 0.0f);
	Pos[1] = Vec3(1.0f, 1.0f, 0.0f);
	Pos[2] = Vec3(-1.0f, -1.0f, 0.0f);
	Pos[3] = Vec3(1.0f, -1.0f, 0.0f);

	Pos[4] = Vec3(-1.0f, 1.0f, 1.0f);
	Pos[5] = Vec3(1.0f, 1.0f, 1.0f);
	Pos[6] = Vec3(-1.0f, -1.0f, 1.0f);
	Pos[7] = Vec3(1.0f, -1.0f, 1.0f);
}


AIMFrustrum::~AIMFrustrum()
{
}

void AIMFrustrum::Update(const Matrix & _InvVP)
{
	Vec3 TmpPos[8];

	for (int i = 0; i < 8; i++)
	{
		TmpPos[i] = Pos[i].TransformCoord(_InvVP);
	}

	Plane[FD_LEFT] = DirectX::XMPlaneFromPoints(TmpPos[4].Convert(), TmpPos[0].Convert(), TmpPos[2].Convert());

	Plane[FD_RIGHT] = DirectX::XMPlaneFromPoints(TmpPos[1].Convert(), TmpPos[5].Convert(), TmpPos[7].Convert());

	Plane[FD_TOP] = DirectX::XMPlaneFromPoints(TmpPos[4].Convert(), TmpPos[5].Convert(), TmpPos[1].Convert());

	Plane[FD_BOTTOM] = DirectX::XMPlaneFromPoints(TmpPos[2].Convert(), TmpPos[3].Convert(), TmpPos[7].Convert());

	Plane[FD_NEAR] = DirectX::XMPlaneFromPoints(TmpPos[0].Convert(), TmpPos[1].Convert(), TmpPos[3].Convert());

	Plane[FD_FAR] = DirectX::XMPlaneFromPoints(TmpPos[5].Convert(), TmpPos[4].Convert(), TmpPos[6].Convert());
}

bool AIMFrustrum::FrustrumInPoint(const Vec3 & _Pos)
{
	for (int i = 0; i < FD_END; i++)
	{
		float Dist = DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(Plane[i].Convert(), _Pos.Convert()));

		if (Dist > 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool AIMFrustrum::FrustrumInSphere(const Vec3 & _Center, float _Radius)
{
	for (int i = 0; i < FD_END; i++)
	{
		float Dist = DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(Plane[i].Convert(), _Center.Convert()));

		if (Dist - _Radius > 0.0f)
		{
			return false;
		}
	}

	return true;
}


