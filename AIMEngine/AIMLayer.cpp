#include "AIMLayer.h"
#include "AIMScene.h"
#include "AIMObject.h"
#include "RenderManager.h"

AIMLayer::AIMLayer()
{
}


AIMLayer::~AIMLayer()
{
	ObjList.clear();
}

int AIMLayer::GetOrder() const
{
	return Order;
}

void AIMLayer::SetOrder(int _Order)
{
	Order = _Order;

	Scene->SortLayer();
}

bool AIMLayer::Init()
{
	return true;
}

int AIMLayer::Input(float _Time)
{
	std::list<Ezptr<AIMObject>>::iterator StartIter = ObjList.begin();
	std::list<Ezptr<AIMObject>>::iterator EndIter = ObjList.end();

	for (; StartIter != EndIter ; )
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ObjList.erase(StartIter);
			continue;
		}

		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->Input(_Time);
		++StartIter;
	}

	return 0;
}

int AIMLayer::Update(float _Time)
{
	std::list<Ezptr<AIMObject>>::iterator StartIter = ObjList.begin();
	std::list<Ezptr<AIMObject>>::iterator EndIter = ObjList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ObjList.erase(StartIter);
			continue;
		}

		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->Update(_Time);
		++StartIter;
	}

	return 0;
}

int AIMLayer::LateUpdate(float _Time)
{
	std::list<Ezptr<AIMObject>>::iterator StartIter = ObjList.begin();
	std::list<Ezptr<AIMObject>>::iterator EndIter = ObjList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ObjList.erase(StartIter);
			continue;
		}

		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->LateUpdate(_Time);
		++StartIter;
	}

	return 0;
}

int AIMLayer::Collision(float _Time)
{
	std::list<Ezptr<AIMObject>>::iterator StartIter = ObjList.begin();
	std::list<Ezptr<AIMObject>>::iterator EndIter = ObjList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ObjList.erase(StartIter);
			continue;
		}

		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->Collision(_Time);
		++StartIter;
	}

	return 0;
}

int AIMLayer::Render(float _Time)
{
	std::list<Ezptr<AIMObject>>::iterator StartIter = ObjList.begin();
	std::list<Ezptr<AIMObject>>::iterator EndIter = ObjList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ObjList.erase(StartIter);
			continue;
		}

		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		RenderManager::AddRenderObject(*StartIter);
		++StartIter;
	}

	/*StartIter = ObjList.begin();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ObjList.erase(StartIter);
			continue;
		}

		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->Render(_Time);
		++StartIter;
	}*/

	return 0;
}

void AIMLayer::AddObject(Ezptr<AIMObject> _Obj)
{
	_Obj->SetScene(Scene);
	_Obj->SetLayer(this);

	_Obj->Start();

	ObjList.push_back(_Obj);
}
