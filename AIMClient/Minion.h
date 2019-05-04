#pragma once

#include <UserComponent.h>
class Minion : public UserComponent
{
public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual Minion* Clone() const;

public:
	Minion();
	Minion(const Minion& _Other);
	~Minion();
};

