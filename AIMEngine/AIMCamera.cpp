#include "AIMCamera.h"
#include "AIMTransform.h"


AIMCamera::AIMCamera()
{
	CT = CT_CAMERA;
}

AIMCamera::AIMCamera(const AIMCamera & _Other) : View(_Other.View), Projection(_Other.Projection), CameraType(_Other.CameraType),
Width(_Other.Width), Height(_Other.Height), Angle(_Other.Angle), Near(_Other.Near), Far(_Other.Far), AIMComponent(_Other)
{

}


AIMCamera::~AIMCamera()
{
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
		Projection = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, (float)Width, (float)Height, 0.0f, 0.0f, Far);
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
	return 0;
}

int AIMCamera::LateUpdate(float _Time)
{
	if (Transform->GetUpdate() == true)
	{
		// ������� ���ϴ� �����̴�.
		// ���������δ�
		// ī�޶��� ��ġ���� Eye
		// ī�޶� ���� ���⺤�� At
		// ī�޶��� ���� ���ϴ� ���⺤�� Up

		// 1. z���� ���Ѵ�
		//	1-1. At - Eye
		//	1-2. Normalize

		// 2. x���� ���Ѵ�
		//	2-1. z��� Up ����
		//	2-2. Normalize

		// (�ʿ��ϴٸ�) 3. y���� ���Ѵ�
		//	3-1. ���� z��� x���� ����
		//	3-2. Normalize

		// 4. ����� ī�޶���� �������κ��� ī�޶���� �Ÿ����͸� �����Ѵ�
		//	4-1. Eye�� �� XYZ�� ���� ����
		// ���ǻ� D.x D.y D.z��� ��

		// 5. ������� �����Ѵ�
		// X.x Y.x Z.x 0
		// X.y Y.y Z.Y 0
		// X.z Y.z Z.z 0
		// -D.x -D.y -D.z 0

		View.Identify();

		for (int i = 0; i < AXIS_END; i++)
		{
			// ī�޶� ��ġ������ ����Ŀ� ��´�.
			memcpy(&View[i][0], &Transform->GetWorldAxis((Axis)i), sizeof(Vec3));
		}
		
		// ����� ��ġ(��� ���� �ٲ۴�)
		View.Transpose();

		for (int i = 0; i < AXIS_END; i++)
		{
			// ������� ������ ���� ���� [ �� �������Ϳ� ��ġ���͸� ������ �� * -1.0f ]
			View[3][i] = -Transform->GetWorldPosition().Dot(Transform->GetWorldAxis((Axis)i));
		}
	}

	return 0;
}

int AIMCamera::Collision(float _Time)
{
	return 0;
}

int AIMCamera::PrevRender(float _Time)
{
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
