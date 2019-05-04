#pragma once

#include "UserComponent.h"
class Player : public UserComponent
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
	virtual Player* Clone() const;

public:
	void MoveFront(float _Scale, float _Time);
	void Fire(float _Time);
	void Test(float _Time);

public:
	Player();
	Player(const Player& _Other);
	~Player();
};

