#include "AIMObject.h"
#include "AIMComponent.h"
#include "AIMLayer.h"
#include "AIMTransform.h"

AIMObject::AIMObject()
{
	Transform = AddComponent<AIMTransform>("Transform");
}

AIMObject::AIMObject(const AIMObject & _Other)
{
	*this = _Other;

	// refcounter 1로 만들어줘야하나?
}

AIMObject::~AIMObject()
{
	ComList.clear();
}

Ezptr<AIMObject> AIMObject::CreateObject(const std::string & _Name, Ezptr<AIMLayer> _Layer)
{
	Ezptr<AIMObject> Obj = new AIMObject;

	if (Obj->Init() == false)
	{
		return nullptr;
	}

	Obj->SetNameTag(_Name.c_str());

	if (_Layer != nullptr)
	{
		_Layer->AddObject(Obj);
	}

	return Obj;
}

AIMScene * AIMObject::GetScene() const
{
	return Scene;
}

AIMLayer * AIMObject::GetLayer() const
{
	return Layer;
}

Ezptr<AIMTransform> AIMObject::GetTransform() const
{
	return Transform;
}

void AIMObject::SetScene(AIMScene * _Scene)
{
	Scene = _Scene;
}

void AIMObject::SetLayer(AIMLayer * _Layer)
{
	Layer = _Layer;
}

void AIMObject::Start()
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->Start();
	}
}

bool AIMObject::Init()
{
	return true;
}

int AIMObject::Input(float _Time)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ComList.erase(StartIter);
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

int AIMObject::Update(float _Time)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ComList.erase(StartIter);
			continue;
		}

		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		if (Transform == (*StartIter) && Transform->GetUpdate() == true)
		{
			(*StartIter)->Update(_Time);
		}
		else
		{
			(*StartIter)->Update(_Time);
		}
		++StartIter;
	}

	return 0;
}

int AIMObject::LateUpdate(float _Time)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ComList.erase(StartIter);
			continue;
		}

		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		if (Transform == (*StartIter) && Transform->GetUpdate() == true)
		{
			(*StartIter)->LateUpdate(_Time);
		}
		else
		{
			(*StartIter)->LateUpdate(_Time);
		}
		++StartIter;
	}

	return 0;
}

int AIMObject::Collision(float _Time)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ComList.erase(StartIter);
			continue;
		}

		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		if (Transform == (*StartIter) && Transform->GetUpdate() == true)
		{
			(*StartIter)->Update(_Time);
		}
		else
		{
			(*StartIter)->Update(_Time);
		}
		++StartIter;
	}

	return 0;
}

int AIMObject::PrevRender(float _Time)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ComList.erase(StartIter);
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

	return 0;
}

int AIMObject::Render(float _Time)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = ComList.erase(StartIter);
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

	return 0;
}

AIMObject* AIMObject::Clone() const
{
	return nullptr;
}

AIMComponent* AIMObject::AddComponent(Ezptr<AIMComponent> _Com)
{
	_Com->SetScene(Scene);
	_Com->SetLayer(Layer);
	_Com->SetAIMObject(this);
	_Com->Transform = Transform;
	
	ComList.push_back(_Com);

	return _Com;
}
