#pragma once

#include "AIMComponent.h"
class Engine_DLL __declspec(align(16)) AIMCamera : public AIMComponent
{
	friend class AIMObject;

private:
	Matrix View;
	Matrix Projection;
	CamType CameraType;
	UINT Width = 0;
	UINT Height = 0;
	float Angle = 0.0f;
	float Near = 0.0f;
	float Far = 0.0f;

public:
	void SetCameraInfo(CamType _Type, UINT _Widht, UINT _Height, float _Angle, float _Near, float _Far);
	void SetCameraType(CamType _Type);

public:
	Matrix GetView() const
	{
		return View;
	}

	Matrix GetProjection() const
	{
		return Projection;
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
	virtual AIMCamera* Clone() const;

private:
	AIMCamera();
	AIMCamera(const AIMCamera& _Other);
	~AIMCamera();
};

