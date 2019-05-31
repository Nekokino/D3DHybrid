#include "AIMCamera.h"
#include "AIMTransform.h"
#include "AIMFrustrum.h"


AIMCamera::AIMCamera()
{
	CT = CT_CAMERA;
	Frustrum = new AIMFrustrum;
}

AIMCamera::AIMCamera(const AIMCamera & _Other) : View(_Other.View), Projection(_Other.Projection), CameraType(_Other.CameraType),
Width(_Other.Width), Height(_Other.Height), Angle(_Other.Angle), Near(_Other.Near), Far(_Other.Far), AIMComponent(_Other), bUpdate(_Other.bUpdate)
{
	Frustrum = new AIMFrustrum;
}


AIMCamera::~AIMCamera()
{
	if (Frustrum != nullptr)
	{
		delete Frustrum;
		Frustrum = nullptr;
	}
}

void AIMCamera::SetCameraInfo(CamType _Type, UINT _Width, UINT _Height, float _Angle, float _Near, float _Far)
{
	CameraType = _Type;
	Width = _Width;
	Height = _Height;
	Angle = _Angle;
	Near = _Near;
	Far = _Far;
	SetCameraType(_Type);
}

void AIMCamera::SetCameraType(CamType _Type)
{
	switch (_Type)
	{
	case CT_PERS:
		Projection = DirectX::XMMatrixPerspectiveFovLH(MATH_D2R * Angle, (float)Width / (float)Height, Near, Far);
		break;
	case CT_ORTH:
		Projection = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, (float)Width, 0.0f, (float)Height, 0.0f, Far);
		break;
	default:
		break;
	}
}

void AIMCamera::Start()
{
}

bool AIMCamera::Init()
{
	return true;
}

int AIMCamera::Input(float _Time)
{
	return 0;
}

int AIMCamera::Update(float _Time)
{
	bUpdate = false;
	return 0;
}

int AIMCamera::LateUpdate(float _Time)
{
	return 0;
}

int AIMCamera::Collision(float _Time)
{
	return 0;
}

int AIMCamera::PrevRender(float _Time)
{
	if (Transform->GetUpdate() == true && Transform->GetUI() == false)
	{
		bUpdate = true;
		// 뷰행렬을 구하는 과정이다.
		// 정석적으로는
		// 카메라의 위치벡터 Eye
		// 카메라가 보는 방향벡터 At
		// 카메라의 위를 향하는 방향벡터 Up

		// 1. z축을 구한다
		//	1-1. At - Eye
		//	1-2. Normalize

		// 2. x축을 구한다
		//	2-1. z축과 Up 외적
		//	2-2. Normalize

		// (필요하다면) 3. y축을 구한다
		//	3-1. 구한 z축과 x축을 내적
		//	3-2. Normalize

		// 4. 연산된 카메라축과 원점으로부터 카메라까의 거리벡터를 내적한다
		//	4-1. Eye와 축 XYZ를 세번 내적
		// 편의상 D.x D.y D.z라고 함

		// 5. 뷰행렬을 구성한다
		// X.x Y.x Z.x 0
		// X.y Y.y Z.Y 0
		// X.z Y.z Z.z 0
		// -D.x -D.y -D.z 0

		View.Identify();

		for (int i = 0; i < AXIS_END; i++)
		{
			// 카메라 위치정보를 뷰행렬에 담는다.
			memcpy(&View[i][0], &Transform->GetWorldAxis((Axis)i), sizeof(Vec3));
		}

		// 뷰행렬 전치(행과 열을 바꾼다)
		View.Transpose();

		for (int i = 0; i < AXIS_END; i++)
		{
			// 뷰행렬의 마지막 행의 값은 [ 각 기저벡터와 위치벡터를 내적한 값 * -1.0f ]
			View[3][i] = -Transform->GetWorldPosition().Dot(Transform->GetWorldAxis((Axis)i));
		}
	}

	Matrix VP = View * Projection;
	VP.Inverse();
	Frustrum->Update(VP);

	return 0;
}

int AIMCamera::Render(float _Time)
{
	return 0;
}

AIMCamera * AIMCamera::Clone() const
{
	return new AIMCamera(*this);
}

bool AIMCamera::FrustrumInPoint(const Vec3& _Pos)
{
	return Frustrum->FrustrumInPoint(_Pos);
}

bool AIMCamera::FrustrumInSphere(const Vec3& _Center, float _Radius)
{
	return Frustrum->FrustrumInSphere(_Center, _Radius);
}