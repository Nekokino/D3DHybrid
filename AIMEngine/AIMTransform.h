#pragma once

#include "AIMComponent.h"
#include "RefStruct.h"

class Engine_DLL __declspec(align(16)) AIMTransform : public AIMComponent
{
	friend class AIMObject;

private:
	bool bStatic = false;
	bool bUpdate = true;
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

private:
	Vec3 LocalScale;
	Vec3 LocalRotation;
	Vec3 LocalPosition;

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

private:
	Matrix LocalScaleMat;
	Matrix LocalRotationMat;
	Matrix LocalPositionMat;
	Matrix LocalMat;

public:
	Matrix GetLocalMatrix() const
	{
		return LocalMat;
	}

private:
	Vec3 WorldScale;
	Vec3 WorldRotation;
	Vec3 WorldPosition;
	Vec3 WorldAxis[AXIS_END];

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

private:
	Matrix WorldScaleMat;
	Matrix WorldRotationMat;
	Matrix WorldPositionMat;
	Matrix WorldParentMat;
	Matrix WorldMat;

public:
	void SetParentMatrix(const Matrix& _Parent);

public:
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

private:
	AIMTransform();
	AIMTransform(const AIMTransform& _Other);
	~AIMTransform();
};

