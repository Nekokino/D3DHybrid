#pragma once

#include <UserComponent.h>
#include <AIMAnimation.h>
#include <AIMCollider.h>
#include <AIMNavigation.h>
class Minion : public UserComponent
{
private:
	Ezptr<AIMAnimation> Animation;
	Ezptr<AIMNavigation> Navigation;

public:
	void Trace(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest, float _Time);
	void TraceExit(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest, float _Time);

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

