#pragma once
#include "AIMCollider.h"
class Engine_DLL AIMColliderSphere : public AIMCollider
{
	friend class AIMObject;

private:
	SphereInfo Relative;
	SphereInfo Info;
	bool ScaleEnable = true;

public:
	void SetSphereInfo(const Vec3& _Center, float _Radius);
	void SetScaleEnable(bool _Enable);
	SphereInfo GetInfo() const;

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMColliderSphere* Clone() const;
	
	virtual bool Collision(Ezptr<AIMCollider> _Dest);

protected:
	AIMColliderSphere();
	AIMColliderSphere(const AIMColliderSphere& _Other);
	virtual ~AIMColliderSphere();
};

