#pragma once

#include "AIMComponent.h"
class Engine_DLL UserComponent : public AIMComponent
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
	virtual UserComponent* Clone() const;

protected:
	UserComponent();
	UserComponent(const UserComponent& _Other);
	virtual ~UserComponent() = 0 {}
};

