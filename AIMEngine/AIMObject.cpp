#include "AIMObject.h"
#include "AIMComponent.h"
#include "AIMLayer.h"
#include "AIMTransform.h"
#include "AIMScene.h"
#include "SceneManager.h"
#include "AIMSampler.h"
#include "AIMCollider.h"
#include "CollisionManager.h"

std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<AIMObject>>> AIMObject::PrototypeMap;

AIMObject::AIMObject()
{
	Transform = AddComponent<AIMTransform>("Transform");
}

AIMObject::AIMObject(const AIMObject & _Other) : Scene(nullptr), Layer(nullptr), Transform(_Other.Transform)
{
	Scene = nullptr;
	Layer = nullptr;

	Instancing = _Other.Instancing;
	Frustrum = _Other.Frustrum;

	ComList.clear();
	ColliderList.clear();
	StartList.clear();

	std::list<Ezptr<AIMComponent>>::const_iterator StartIter = _Other.ComList.begin();
	std::list<Ezptr<AIMComponent>>::const_iterator EndIter = _Other.ComList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		Ezptr<AIMComponent> Com = (*StartIter)->Clone();

		if (Com->GetComType() == CT_TRANSFORM)
		{
			Transform = Com;
		}

		else if (Com->GetComType() == CT_COLLIDER)
		{
			ColliderList.push_back(Com);
		}

		Com->SetAIMObject(this);
		Com->Transform = Transform;
		ComList.push_back(Com);
		StartList.push_back(Com);
	}

	RG = _Other.RG;
	LifeTime = _Other.LifeTime;

	// refcounter 1로 만들어줘야하나?
}

AIMObject::~AIMObject()
{
	ComList.clear();
	ColliderList.clear();
	StartList.clear();
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

Ezptr<AIMObject> AIMObject::CreatePrototype(const std::string & _Name, AIMScene* _Scene)
{
	Ezptr<AIMObject> Obj = new AIMObject;

	if (Obj->Init() == false)
	{
		return nullptr;
	}

	Obj->SetNameTag(_Name.c_str());

	if (_Scene == nullptr)
	{
		_Scene = SceneManager::GetScene();
	}

	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<AIMObject>>>::iterator Iter = PrototypeMap.find(_Scene);

	if (Iter == PrototypeMap.end())
	{
		std::unordered_map<std::string, Ezptr<AIMObject>> Map;
		PrototypeMap.insert(std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<AIMObject>>>::value_type(_Scene, Map));
		Iter = PrototypeMap.find(_Scene);
	}

	Obj->Scene = _Scene;

	Iter->second.insert(std::unordered_map<std::string, Ezptr<AIMObject>>::value_type(_Name, Obj));

	return Obj;
}

Ezptr<AIMObject> AIMObject::CreateClone(const std::string & _Name, AIMScene* _Scene, Ezptr<AIMLayer> _Layer)
{
	Ezptr<AIMObject> ProtoObj = FindPrototype(_Name, _Scene);

	if (ProtoObj == nullptr)
	{
		return nullptr;
	}

	Ezptr<AIMObject> Obj = ProtoObj->Clone();

	if (_Layer != nullptr)
	{
		_Layer->AddObject(Obj);
	}

	return Obj;
}

void AIMObject::RemovePrototype(AIMScene* _Scene)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<AIMObject>>>::iterator FindIter = PrototypeMap.find(_Scene);

	if (FindIter == PrototypeMap.end())
	{
		return;
	}

	FindIter->second.clear();

	PrototypeMap.erase(FindIter);
}

void AIMObject::RemovePrototype(AIMScene* _Scene, const std::string & _Prototype)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<AIMObject>>>::iterator FindIter = PrototypeMap.find(_Scene);

	if (FindIter == PrototypeMap.end())
	{
		return;
	}

	std::unordered_map<std::string, Ezptr<AIMObject>>::iterator FindIter2 = FindIter->second.find(_Prototype);

	if (FindIter2 == FindIter->second.end())
	{
		return;
	}

	FindIter->second.erase(FindIter2);
}

Ezptr<AIMObject> AIMObject::FindPrototype(const std::string & _Name, AIMScene* _Scene)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<AIMObject>>>::iterator FindIter = PrototypeMap.find(_Scene);

	if (FindIter == PrototypeMap.end())
	{
		return nullptr;
	}

	std::unordered_map<std::string, Ezptr<AIMObject>>::iterator FindIter2 = FindIter->second.find(_Name);

	if (FindIter2 == FindIter->second.end())
	{
		return nullptr;
	}

	return FindIter2->second;
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

	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		(*StartIter)->Scene = _Scene;
	}
}

void AIMObject::SetLayer(AIMLayer * _Layer)
{
	Layer = _Layer;

	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->Layer = _Layer;
	}
}

void AIMObject::SetRenderGroup(RenderGroup _RG)
{
	RG = _RG;
}

void AIMObject::SetLifeTime(float _Time)
{
	LifeTime = _Time;
}

void AIMObject::SetInstancingEnable(bool _Enable)
{
	Instancing = _Enable;
}

void AIMObject::SetFrustrumCulling(bool _Culling)
{
	Frustrum = _Culling;
}

const std::list<Ezptr<AIMCollider>>* AIMObject::GetColliderList() const
{
	return &ColliderList;
}

void AIMObject::Start()
{
	if (StartList.empty() == true)
	{
		return;
	}

	std::list<Ezptr<AIMComponent>>::iterator StartIter = StartList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = StartList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->Start();
	}

	StartList.clear();
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
	Start();

	Instancing = false;
	Frustrum = false;

	if (LifeTime != -1.0f)
	{
		LifeTime -= _Time;

		if (LifeTime <= 0.0f)
		{
			SetAlive(false);
			return 0;
		}
	}

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

	Transform->RotationLookAt();

	return 0;
}

int AIMObject::LateUpdate(float _Time)
{
	Start();

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
	Start();

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
	Start();

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

		else if ((*StartIter)->GetComType() == CT_COLLIDER)
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
	if (Instancing == true)
	{
		return 0;
	}

	PrevRender(_Time);

	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter ;)
	{
		if (false == (*StartIter)->IsAlive())
		{
			StartIter = ComList.erase(StartIter);
			continue;
		}

		else if (false == (*StartIter)->IsEnable())
		{
			++StartIter;
			continue;
		}

		else if ((*StartIter)->GetComType() == CT_COLLIDER)
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
	return new AIMObject(*this);
}

bool AIMObject::CheckComponent(const std::string & _Name)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter)->GetNameTag() == _Name)
		{
			return true;
		}
	}

	return false;
}

bool AIMObject::CheckComponent(ComType _Type)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter)->GetComType() == _Type)
		{
			return true;
		}
	}

	return false;
}

void AIMObject::EraseComponent(const std::string & _Name)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (;  StartIter != EndIter ; ++StartIter)
	{
		if ((*StartIter)->GetNameTag() == _Name)
		{
			ComList.erase(StartIter);
			break;
		}
	}

	std::list<Ezptr<AIMCollider>>::iterator StartIter2 = ColliderList.begin();
	std::list<Ezptr<AIMCollider>>::iterator EndIter2 = ColliderList.end();

	for (; StartIter2 != EndIter2; ++StartIter2)
	{
		if ((*StartIter2)->GetNameTag() == _Name)
		{
			ColliderList.erase(StartIter2);
			return;
		}
	}
}

void AIMObject::EraseComponent(ComType _Type)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter)->GetComType() == _Type)
		{
			ComList.erase(StartIter);
			break;
		}
	}

	std::list<Ezptr<AIMCollider>>::iterator StartIter2 = ColliderList.begin();
	std::list<Ezptr<AIMCollider>>::iterator EndIter2 = ColliderList.end();

	for (; StartIter2 != EndIter2; ++StartIter2)
	{
		if ((*StartIter2)->GetComType() == _Type)
		{
			ColliderList.erase(StartIter2);
			return;
		}
	}
}

void AIMObject::EraseComponent(Ezptr<AIMComponent> _Com)
{
	std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
	std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter) == _Com)
		{
			ComList.erase(StartIter);
			break;
		}
	}

	std::list<Ezptr<AIMCollider>>::iterator StartIter2 = ColliderList.begin();
	std::list<Ezptr<AIMCollider>>::iterator EndIter2 = ColliderList.end();

	for (; StartIter2 != EndIter2; ++StartIter2)
	{
		if ((*StartIter2) == _Com)
		{
			ColliderList.erase(StartIter2);
			return;
		}
	}
}

AIMComponent* AIMObject::AddComponent(Ezptr<AIMComponent> _Com)
{
	_Com->SetScene(Scene);
	_Com->SetLayer(Layer);
	_Com->SetAIMObject(this);
	_Com->Transform = Transform;

	if (_Com->GetComType() == CT_COLLIDER)
	{
		ColliderList.push_back(_Com);
	}

	StartList.push_back(_Com);
	
	ComList.push_back(_Com);

	return _Com;
}
