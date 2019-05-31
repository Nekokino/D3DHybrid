#include "AIMTransform.h"
#include "AIMDevice.h"
#include "ShaderManager.h"
#include "AIMObject.h"
#include "AIMScene.h"
#include "AIMCamera.h"

AIMTransform::AIMTransform()
{
	CT = CT_TRANSFORM;

	LocalRelativeView = Vec3::Axis[AXIS_Z];
	LocalView = LocalRelativeView;
	WorldView = LocalRelativeView;
	LookAtAxis = LA_ALL;
}

AIMTransform::~AIMTransform()
{
	LookAtTransform = nullptr;
}

AIMTransform::AIMTransform(const AIMTransform & _Other) : bStatic(_Other.bStatic), bUpdate(_Other.bUpdate), ConstBuffer(_Other.ConstBuffer),
LocalScale(_Other.LocalScale), LocalRotation(_Other.LocalRotation), LocalPosition(_Other.LocalPosition), LocalRelativeView(_Other.LocalRelativeView),
LocalView(_Other.LocalView), LocalScaleMat(_Other.LocalScaleMat), LocalRotationMat(_Other.LocalRotationMat), LocalMat(_Other.LocalMat), LocalPositionMat(_Other.LocalPositionMat),
WorldScale(_Other.WorldScale), WorldRotation(_Other.WorldRotation), WorldPosition(_Other.WorldPosition), WorldView(_Other.WorldView),
LookAtTransform(_Other.LookAtTransform), LookAtAxis(_Other.LookAtAxis), WorldScaleMat(_Other.WorldScaleMat), WorldRotationMat(_Other.WorldRotationMat),
WorldPositionMat(_Other.WorldPositionMat), WorldParentMat(_Other.WorldParentMat), WorldMat(_Other.WorldMat), bUI(_Other.bUI),
AIMComponent(_Other)
{
	for (int i = 0; i < AXIS_END; i++)
	{
		WorldAxis[i] = _Other.WorldAxis[i];
	}

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

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::SetWorldRotation(const Vec3 & _Rotation)
{
	WorldRotation = _Rotation;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::SetWorldRotationX(float _x)
{
	WorldRotation.x = _x;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::SetWorldRotationY(float _y)
{
	WorldRotation.y = _y;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::SetWorldRotationZ(float _z)
{
	WorldRotation.z = _z;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

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
	MoveVec += _Position - WorldPosition;
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

	LocalView = LocalRelativeView.TransformNormal(LocalRotationMat);
	LocalView.Normalize();

	bUpdate = true;
}

void AIMTransform::SetLocalRotation(const Vec3 & _Rotation)
{
	LocalRotation = _Rotation;

	LocalRotationMat.Rotation(LocalRotation);

	LocalView = LocalRelativeView.TransformNormal(LocalRotationMat);
	LocalView.Normalize();

	bUpdate = true;
}

void AIMTransform::SetLocalRotationX(float _x)
{
	LocalRotation.x = _x;

	LocalRotationMat.Rotation(LocalRotation);

	LocalView = LocalRelativeView.TransformNormal(LocalRotationMat);
	LocalView.Normalize();

	bUpdate = true;
}

void AIMTransform::SetLocalRotationY(float _y)
{
	LocalRotation.y = _y;

	LocalRotationMat.Rotation(LocalRotation);

	LocalView = LocalRelativeView.TransformNormal(LocalRotationMat);
	LocalView.Normalize();

	bUpdate = true;
}

void AIMTransform::SetLocalRotationZ(float _z)
{
	LocalRotation.z = _z;

	LocalRotationMat.Rotation(LocalRotation);

	LocalView = LocalRelativeView.TransformNormal(LocalRotationMat);
	LocalView.Normalize();

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
	LocalScale = Vec3(1.0f, 1.0f, 1.0f);
	WorldScale = Vec3(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < AXIS_END; i++)
	{
		WorldAxis[i] = Vec3::Axis[i];
	}

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
	Ezptr<AIMCamera> Cam;

	if (bUI == true)
	{
		Cam = Scene->GetUICamera();
	}
	else
	{
		Cam = Scene->GetMainCamera();
	}

	if (true == bUpdate || Cam->GetUpdate() == true)
	{
		ConstBuffer.WorldRotation = LocalRotationMat * WorldRotationMat;
		ConstBuffer.WVRotation = ConstBuffer.WorldRotation * Cam->GetView();
		ConstBuffer.World = LocalMat * WorldMat;
		ConstBuffer.View = Cam->GetView();
		ConstBuffer.Projection = Cam->GetProjection();
		ConstBuffer.InvProjection = ConstBuffer.Projection;
		ConstBuffer.InvProjection.Inverse();
		ConstBuffer.WV = ConstBuffer.World * ConstBuffer.View;
		ConstBuffer.WVP = ConstBuffer.WV * ConstBuffer.Projection;
		ConstBuffer.VP = ConstBuffer.View * ConstBuffer.Projection;

		ConstBuffer.WorldRotation.Transpose();
		ConstBuffer.WVRotation.Transpose();
		ConstBuffer.World.Transpose();
		ConstBuffer.View.Transpose();
		ConstBuffer.Projection.Transpose();
		ConstBuffer.InvProjection.Transpose();
		ConstBuffer.WV.Transpose();
		ConstBuffer.WVP.Transpose();
		ConstBuffer.VP.Transpose();
	}

	ShaderManager::UpdateConstBuffer("Transform", &ConstBuffer);

	return 0;
}

int AIMTransform::Render(float _Time)
{
	MoveVec = Vec3::Zero;

	bUpdate = false;

	return 0;
}

void AIMTransform::Move(Axis _Axis, float _Speed, float _Time)
{
	MoveVec += WorldAxis[_Axis] * _Speed * _Time;
	WorldPosition += WorldAxis[_Axis] * _Speed * _Time;

	WorldPositionMat.Translation(WorldPosition);

	bUpdate = true;
}

void AIMTransform::MoveBack()
{
	WorldPosition -= MoveVec;
	MoveVec = Vec3::Zero;

	WorldPositionMat.Translation(WorldPosition);

	bUpdate = true;
}

void AIMTransform::SetLocalRelativeView(float _x, float _y, float _z)
{
	LocalRelativeView = Vec3(_x, _y, _z);

	LocalView = LocalRelativeView.TransformNormal(LocalRotationMat);
	LocalView.Normalize();
}

void AIMTransform::SetLocalRelativeView(const Vec3 & _View)
{
	LocalRelativeView = _View;

	LocalView = LocalRelativeView.TransformNormal(LocalRotationMat);
	LocalView.Normalize();
}

void AIMTransform::Move(const Vec3 & _Dir, float _Speed, float _Time)
{
	MoveVec += _Dir * _Speed * _Time;
	WorldPosition += _Dir * _Speed * _Time;

	WorldPositionMat.Translation(WorldPosition);

	bUpdate = true;
}

void AIMTransform::Move(const Vec3& _Move)
{
	MoveVec += _Move;
	WorldPosition += _Move;

	WorldPositionMat.Translation(WorldPosition);

	bUpdate = true;
}

void AIMTransform::Rotation(const Vec3& _Rot, float _Time)
{
	WorldRotation += _Rot * _Time;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::Rotation(const Vec3& _Rot)
{
	WorldRotation += _Rot;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::RotationX(float _Speed, float _Time)
{
	WorldRotation.x += _Speed * _Time;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::RotationX(float _Speed)
{
	WorldRotation.x += _Speed;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::RotationY(float _Speed, float _Time)
{
	WorldRotation.y += _Speed * _Time;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::RotationY(float _Speed)
{
	WorldRotation.y += _Speed;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::RotationZ(float _Speed, float _Time)
{
	WorldRotation.z += _Speed * _Time;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::RotationZ(float _Speed)
{
	WorldRotation.z += _Speed;

	WorldRotationMat.Rotation(WorldRotation);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::LookAt(Ezptr<AIMObject> _Obj)
{
	LookAtTransform = _Obj->GetTransform();
}

void AIMTransform::LookAt(Ezptr<AIMComponent> _Com)
{
	if (_Com->GetComType() == CT_TRANSFORM)
	{
		LookAtTransform = _Com;
	}

	else
	{
		LookAtTransform = _Com->GetTransform();
	}
}

void AIMTransform::RemoveLookAt()
{
	LookAtTransform = nullptr;
}

void AIMTransform::RotationLookAt()
{
	if (LookAtTransform == nullptr)
	{
		return;
	}

	RotationLookAt(LookAtTransform->GetWorldPosition());
}

void AIMTransform::RotationLookAt(Vec3 _LookAt)
{
	Vec3 View = LocalView;
	Vec3 Pos = WorldPosition;

	switch (LookAtAxis)
	{
	case LA_X:
		_LookAt.x = 0.0f;
		Pos.x = 0.0f;
		View.x = 0.0f;
		break;
	case LA_Y:
		_LookAt.y = 0.0f;
		Pos.y = 0.0f;
		View.y = 0.0f;
		break;
	case LA_Z:
		_LookAt.z = 0.0f;
		Pos.z = 0.0f;
		View.z = 0.0f;
		break;
	default:
		break;
	}

	Vec3 Dir = _LookAt - Pos;
	Dir.Normalize();
	View.Normalize();

	// 회전축 계산
	Vec3 RotAxis = View.Cross(Dir);
	RotAxis.Normalize();

	float Angle = View.Angle(Dir);

	WorldRotationMat.RotationAxis(Angle, RotAxis);

	ComputeAxis();

	bUpdate = true;
}

void AIMTransform::ComputeAxis()
{
	for (int i = 0; i < AXIS_END; i++)
	{
		WorldAxis[i] = Vec3::Axis[i].TransformNormal(WorldRotationMat);
		WorldAxis[i].Normalize();
	}

	WorldView = LocalView.TransformNormal(WorldRotationMat);
	WorldView.Normalize();
}

void AIMTransform::UIEnable()
{
	bUI = true;
}
