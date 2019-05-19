#pragma once

#include "AIMComponent.h"

class Engine_DLL AIMLight : public AIMComponent
{
	friend class AIMObject;

private:
	LightInfo Info;
	float InAngle;
	float OutAngle;

public:
	LightInfo GetLightInfo() const
	{
		return Info;
	}

public:
	void SetLightType(LightType _Type)
	{
		Info.Type = _Type;
	}

	void SetLightColor(const Vec4& _Dif, const Vec4& _Amb, const Vec4& _Spc)
	{
		Info.Diffuse = _Dif;
		Info.Ambient = _Amb;
		Info.Specular = _Spc;
	}

	void SetLightDistance(float _Dist)
	{
		Info.Distance = _Dist;
	}

	void SetLightAngle(float _In, float _Out)
	{
		InAngle = _In;
		OutAngle = _Out;

		Info.InAngle = cosf(MATH_D2R * InAngle * 0.5f);
		Info.OutAngle = cosf(MATH_D2R * OutAngle * 0.5f);
	}

public:
	void SetShader();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMLight* Clone() const;
private:
	AIMLight();
	AIMLight(const AIMLight& _Other);
	~AIMLight();
};

