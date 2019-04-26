#include "AIMTransform.h"
#include "AIMDevice.h"
#include "ShaderManager.h"

AIMTransform::AIMTransform()
{
	CT = CT_TRANSFORM;
}

AIMTransform::~AIMTransform()
{
}

AIMTransform::AIMTransform(const AIMTransform & _Other)
{
	*this = _Other;
	bUpdate = true;
}

void AIMTransform::SetWorldScale(float _x, float _y, float _z)
{
	WorldScale.x = _x;
	WorldScale.y = _y;
	WorldScale.z = _z;

	WorldScaleMat.Scaling(WorldScale);

	bUpdate = true;
}

void AIMTransform::SetWorldScale(const Vec3 & _Scale)
{
	WorldScale = _Scale;

	WorldScaleMat.Scaling(WorldScale);

	bUpdate = true;
}

void AIMTransform::SetWorldRotation(float _x, float _y, float _z)
{
	WorldRotation.x = _x;
	WorldRotation.y = _y;
	WorldRotation.z = _z;

	WorldRotationMat.Rotation(WorldRotation);

	bUpdate = true;
}

void AIMTransform::SetWorldRotation(const Vec3 & _Rotation)
{
	WorldRotation = _Rotation;

	WorldRotationMat.Rotation(WorldRotation);

	bUpdate = true;
}

void AIMTransform::SetWorldRotationX(float _x)
{
	WorldRotation.x = _x;

	WorldRotationMat.Rotation(WorldRotation);

	bUpdate = true;
}

void AIMTransform::SetWorldRotationY(float _y)
{
	WorldRotation.y = _y;

	WorldRotationMat.Rotation(WorldRotation);

	bUpdate = true;
}

void AIMTransform::SetWorldRotationZ(float _z)
{
	WorldRotation.z = _z;

	WorldRotationMat.Rotation(WorldRotation);

	bUpdate = true;
}

void AIMTransform::SetWorldPosition(float _x, float _y, float _z)
{
	WorldPosition.x = _x;
	WorldPosition.y = _y;
	WorldPosition.z = _z;

	WorldPositionMat.Translation(WorldPosition);

	bUpdate = true;
}

void AIMTransform::SetWorldPosition(const Vec3 & _Position)
{
	WorldPosition = _Position;

	WorldPositionMat.Translation(WorldPosition);

	bUpdate = true;
}

void AIMTransform::SetLocalScale(float _x, float _y, float _z)
{
	LocalScale.x = _x;
	LocalScale.y = _y;
	LocalScale.z = _z;

	LocalScaleMat.Scaling(LocalScale);

	bUpdate = true;
}

void AIMTransform::SetLocalScale(const Vec3 & _Scale)
{
	LocalScale = _Scale;

	LocalScaleMat.Scaling(LocalScale);

	bUpdate = true;
}

void AIMTransform::SetLocalRotation(float _x, float _y, float _z)
{
	LocalRotation.x = _x;
	LocalRotation.y = _y;
	LocalRotation.z = _z;

	LocalRotationMat.Rotation(LocalRotation);

	bUpdate = true;
}

void AIMTransform::SetLocalRotation(const Vec3 & _Rotation)
{
	LocalRotation = _Rotation;

	LocalRotationMat.Rotation(LocalRotation);

	bUpdate = true;
}

void AIMTransform::SetLocalRotationX(float _x)
{
	LocalRotation.x = _x;

	LocalRotationMat.Rotation(LocalRotation);

	bUpdate = true;
}

void AIMTransform::SetLocalRotationY(float _y)
{
	LocalRotation.y = _y;

	LocalRotationMat.Rotation(LocalRotation);

	bUpdate = true;
}

void AIMTransform::SetLocalRotationZ(float _z)
{
	LocalRotation.z = _z;

	LocalRotationMat.Rotation(LocalRotation);

	bUpdate = true;
}

void AIMTransform::SetLocalPosition(float _x, float _y, float _z)
{
	LocalPosition.x = _x;
	LocalPosition.y = _y;
	LocalPosition.z = _z;

	LocalPositionMat.Translation(LocalPosition);

	bUpdate = true;
}

void AIMTransform::SetLocalPosition(const Vec3 & _Position)
{
	LocalPosition = _Position;

	LocalPositionMat.Translation(LocalPosition);

	bUpdate = true;
}

void AIMTransform::SetParentMatrix(const Matrix& _Parent)
{
	WorldParentMat = _Parent;
}

AIMTransform* AIMTransform::Clone() const
{
	return new AIMTransform(*this);
}

void AIMTransform::Start()
{

}

bool AIMTransform::Init()
{
	return true;
}

int AIMTransform::Input(float _Time)
{
	return 0;
}

int AIMTransform::Update(float _Time)
{
	return 0;
}

int AIMTransform::LateUpdate(float _Time)
{

	LocalMat = LocalScaleMat * LocalRotationMat * LocalPositionMat;

	WorldMat = WorldScaleMat * WorldRotationMat * WorldPositionMat * WorldParentMat;

	return 0;
}

int AIMTransform::Collision(float _Time)
{
	return 0;
}

int AIMTransform::PrevRender(float _Time)
{
	if (true == bUpdate)
	{
		ConstBuffer.World = LocalMat * WorldMat;
		ConstBuffer.View = DirectX::XMMatrixLookAtLH(Vec3(0.0f, 0.0f, -5.0f).Convert(), Vec3(0.0f, 0.0f, 0.0f).Convert(), Vec3(0.0f, 1.0f, 0.0f).Convert());
		ConstBuffer.Projection = DirectX::XMMatrixPerspectiveFovLH(MATH_PI / 2.0f, (float)GetDeviceInst->GetResolution().Width / (float)GetDeviceInst->GetResolution().Height, 0.03f, 1000.0f);
		ConstBuffer.WV = ConstBuffer.World * ConstBuffer.View;
		ConstBuffer.WVP = ConstBuffer.WV * ConstBuffer.Projection;

		ConstBuffer.World.Transpose();
		ConstBuffer.View.Transpose();
		ConstBuffer.Projection.Transpose();
		ConstBuffer.WV.Transpose();
		ConstBuffer.WVP.Transpose();
	}

	ShaderManager::UpdateConstBuffer("Transform", &ConstBuffer);

	bUpdate = false;

	return 0;
}

int AIMTransform::Render(float _Time)
{
	return 0;
}
