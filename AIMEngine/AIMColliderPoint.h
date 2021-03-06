#pragma once

#include "AIMCollider.h"
class Engine_DLL AIMColliderPoint : public AIMCollider
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
	virtual AIMColliderPoint* Clone() const;

	virtual bool Collision(Ezptr<AIMCollider> _Dest);

protected:
	AIMColliderPoint();
	AIMColliderPoint(const AIMColliderPoint& _Other);
	virtual ~AIMColliderPoint();
};

