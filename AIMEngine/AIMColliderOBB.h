#pragma once

#include "AIMCollider.h"
class Engine_DLL AIMColliderOBB : public AIMCollider
{
	friend class AIMObject;

private:
	OBBInfo Relative;
	OBBInfo Info;

public:
	void SetOBBInfo(const Vec3& _Center, const Vec3& _HalfLength);
	OBBInfo GetInfo() const
	{
		return Info;
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
	virtual AIMColliderOBB* Clone() const;

	virtual bool Collision(Ezptr<AIMCollider> _Dest);

protected:
	AIMColliderOBB();
	AIMColliderOBB(const AIMColliderOBB& _Other);
	virtual ~AIMColliderOBB();
};

