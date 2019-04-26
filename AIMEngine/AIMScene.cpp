#include "AIMScene.h"
#include "SceneComponent.h"
#include "AIMLayer.h"

AIMScene::AIMScene()
{
}


AIMScene::~AIMScene()
{
	SceneComList.clear();
	LayerList.clear();
}

bool AIMScene::Init()
{
	AddLayer("Default", 0);
	AddLayer("UI", INT_MAX - 2);

	return true;
}

int AIMScene::Input(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
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

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->Input(_Time);

		++StartIterA;
	}

	return 0;
}

int AIMScene::Update(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
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

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->Update(_Time);

		++StartIterA;
	}

	return 0;
}

int AIMScene::LateUpdate(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
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

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->LateUpdate(_Time);

		++StartIterA;
	}

	return 0;
}

int AIMScene::Collision(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
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

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->Collision(_Time);

		++StartIterA;
	}

	return 0;
}

int AIMScene::PrevRender(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->PrevRender(_Time);

		++StartIter;
	}

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->PrevRender(_Time);

		++StartIterA;
	}

	return 0;
}

int AIMScene::Render(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->Render(_Time);

		++StartIter;
	}

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->Render(_Time);

		++StartIterA;
	}

	return 0;
}

void AIMScene::AddLayer(const std::string & _Name, int _Order)
{
	Ezptr<AIMLayer> Layer = new AIMLayer;

	Layer->SetNameTag(_Name.c_str());
	Layer->Scene = this;

	if (Layer->Init() == false)
	{
		return;
	}

	LayerList.push_back(Layer);

	Layer->SetOrder(_Order);
}

void AIMScene::SortLayer()
{
	LayerList.sort(AIMScene::SortFunc);
}

Ezptr<AIMLayer> AIMScene::FindLayer(const std::string & _Name)
{
	std::list<Ezptr<AIMLayer>>::iterator StartIter = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIter = LayerList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter)->GetNameTag() == _Name)
		{
			return *StartIter;
		}
	}

	return nullptr;
}

bool AIMScene::SortFunc(Ezptr<AIMLayer> _Src, Ezptr<AIMLayer> _Dest)
{
	int Src = _Src->GetOrder();
	int Dest = _Dest->GetOrder();

	return Src > Dest;
}
