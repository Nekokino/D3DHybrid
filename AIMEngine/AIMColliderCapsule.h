#pragma once

#include "AIMCollider.h"
class Engine_DLL AIMColliderCapsule : public AIMCollider
{
	friend class AIMObject;

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMColliderCapsule* Clone() const;

	virtual bool Collision(Ezptr<AIMCollider> _Dest);

protected:
	AIMColliderCapsule();
	AIMColliderCapsule(const AIMColliderCapsule& _Other);
	virtual ~AIMColliderCapsule();
};

