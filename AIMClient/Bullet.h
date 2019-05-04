#pragma once

#include <UserComponent.h>
class Bullet : public UserComponent
{
private:
	float Speed = 0.0f;
	float Dist = 0.0f;

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual Bullet* Clone() const;

public:
	Bullet();
	Bullet(const Bullet& _Other);
	~Bullet();
};

