#pragma once

#include "AIMComponent.h"
class Engine_DLL AIMArm : public AIMComponent
{
	friend class AIMObject;

private:
	Ezptr<AIMTransform> Target;
	Vec3 Dist;
	bool MouseEnable = false;

public:
	void SetTarget(Ezptr<AIMObject> _Target);
	void SetTarget(Ezptr<AIMComponent> _Target);
	void SetDist(const Vec3& _Dist);
	void SetMouseEnable(bool _Enable);

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMArm* Clone() const;

protected:
	AIMArm();
	AIMArm(const AIMArm& _Other);
	~AIMArm();
};

