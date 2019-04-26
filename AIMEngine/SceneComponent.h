#pragma once

#include "RefCounter.h"

class Engine_DLL SceneComponent : public RefCounter
{
	friend class AIMScene;

protected:
	// 상호참조의 가능성이 충분하기에 일반 포인터로 들게했다.
	// 어차피 신이 없어지면 얘도 뭘 할수 없는게 맞기때문
	AIMScene* Scene = nullptr;

public:
	virtual bool Init() = 0;
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);

protected:
	SceneComponent();
	virtual ~SceneComponent() = 0;
};

