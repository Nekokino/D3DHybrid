#pragma once

#include "RefCounter.h"

class Engine_DLL SceneComponent : public RefCounter
{
	friend class AIMScene;

protected:
	// ��ȣ������ ���ɼ��� ����ϱ⿡ �Ϲ� �����ͷ� ����ߴ�.
	// ������ ���� �������� �굵 �� �Ҽ� ���°� �±⶧��
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

