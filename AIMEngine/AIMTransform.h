#pragma once

#include "AIMComponent.h"
#include "RefStruct.h"

class Engine_DLL __declspec(align(16)) AIMTransform : public AIMComponent
{
	friend class AIMObject;

private:
	bool bStatic = false;
	bool bUpdate = true;
	bool bUI = false;
	TransformConstBuffer ConstBuffer;

public:
	bool GetStatic() const
	{
		return bStatic;
	}

	bool GetUpdate() const
	{
		return bUpdate;
	}

	bool GetUI() const
	{
		return bUI;
	}

	void UIEnable();

private:
	Vec3 LocalScale;
	Vec3 LocalRotation;
	Vec3 LocalPosition;
	Vec3 LocalRelativeView;
	Vec3 LocalView;

public:
	Vec3 GetLocalScale() const
	{
		return LocalScale;
	}

	Vec3 GetLocalRotation() const
	{
		return LocalRotation;
	}

	Vec3 GetLocalPosition() const
	{
		return LocalPosition;
	}

	Vec3 GetLocalView() const
	{
		return LocalView;
	}

public:
	void SetLocalScale(float _x, float _y, float _z);
	void SetLocalScale(const Vec3& _Scale);
	void SetLocalRotation(float _x, float _y, float _z);
	void SetLocalRotation(const Vec3& _Rotation);
	void SetLocalRotationX(float _x);
	void SetLocalRotationY(float _y);
	void SetLocalRotationZ(float _z);
	void SetLocalPosition(float _x, float _y, float _z);
	void SetLocalPosition(const Vec3& _Position);
	void SetLocalRelativeView(float _x, float _y, float _z);
	void SetLocalRelativeView(const Vec3& _View);

private:
	Matrix LocalScaleMat;
	Matrix LocalRotationMat;
	Matrix LocalPositionMat;
	Matrix LocalMat;

public:
	Matrix GetLocalRotationMatrix() const
	{
		return LocalRotationMat;
	}

	Matrix GetLocalMatrix() const
	{
		return LocalMat;
	}

private:
	Vec3 WorldScale;
	Vec3 WorldRotation;
	Vec3 WorldPosition;
	Vec3 WorldAxis[AXIS_END];
	Vec3 WorldView;
	Vec3 MoveVec;
	Ezptr<AIMTransform> LookAtTransform;
	LookAt_Axis LookAtAxis;
	

public:
	Vec3 GetWorldScale() const
	{
		return WorldScale;
	}

	Vec3 GetWorldRotation() const
	{
		return WorldRotation;
	}

	Vec3 GetWorldPosition() const
	{
		return WorldPosition;
	}

	Vec3 GetWorldAxis(Axis _Axis) const
	{
		return WorldAxis[_Axis];
	}

	Vec3 GetWorldView() const
	{
		return WorldView;
	}

	Vec3 GetMove() const
	{
		return MoveVec;
	}


public:
	void SetWorldScale(float _x, float _y, float _z);
	void SetWorldScale(const Vec3& _Scale);
	void SetWorldRotation(float _x, float _y, float _z);
	void SetWorldRotation(const Vec3& _Rotation);
	void SetWorldRotationX(float _x);
	void SetWorldRotationY(float _y);
	void SetWorldRotationZ(float _z);
	void SetWorldPosition(float _x, float _y, float _z);
	void SetWorldPosition(const Vec3& _Position);
	void SetLookAtAxis(LookAt_Axis _Axis)
	{
		LookAtAxis = _Axis;
	}

private:
	Matrix WorldScaleMat;
	Matrix WorldRotationMat;
	Matrix WorldPositionMat;
	Matrix WorldParentMat;
	Matrix WorldMat;

public:
	void SetParentMatrix(const Matrix& _Parent);

public:
	Matrix GetWorldRotationMatrix() const
	{
		return WorldRotationMat;
	}

	Matrix GetWorldMatrix() const
	{
		return WorldMat;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMTransform* Clone() const;

public:
	void Move(Axis _Axis, float _Speed, float _Time);
	void Move(const Vec3& _Dir, float _Speed, float _Time);
	void Move(const Vec3& _Move);
	void MoveBack();
	void Rotation(const Vec3& _Rot, float _Time);
	void Rotation(const Vec3& _Rot);
	void RotationX(float _Speed, float _Time);
	void RotationX(float _Speed);
	void RotationY(float _Speed, float _Time);
	void RotationY(float _Speed);
	void RotationZ(float _Speed, float _Time);
	void RotationZ(float _Speed);
	void LookAt(Ezptr<AIMObject> _Obj);
	void LookAt(Ezptr<AIMComponent> _Com);
	void RemoveLookAt();
	void RotationLookAt();
	void RotationLookAt(Vec3 _LookAt);

private:
	void ComputeAxis();

private:
	AIMTransform();
	AIMTransform(const AIMTransform& _Other);
	~AIMTransform();
};

