#pragma once

#include "RefCounter.h"
#include "AIMObject.h"

class Engine_DLL AIMLayer : public RefCounter
{
	friend class AIMScene;
private:
	AIMScene* Scene;
	int Order = 0;

public:
	int GetOrder() const;
	void SetOrder(int _Order);
	std::list<Ezptr<AIMObject>> ObjList;

public:
	bool Init();
	int Input(float _Time);
	int Update(float _Time);
	int LateUpdate(float _Time);
	int Collision(float _Time);
	int PrevRender(float _Time);
	int Render(float _Time);

	void AddObject(Ezptr<AIMObject> _Obj);

private:
	AIMLayer();
	~AIMLayer();
};

