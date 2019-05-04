#pragma once

#include "RefCounter.h"
#include "AIMComponent.h"
#include "AIMTransform.h"
#include "AIMScene.h"

class AIMLayer;
class Engine_DLL AIMObject : public RefCounter
{
private:
	static std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<AIMObject>>> PrototypeMap;

public:
	static Ezptr<AIMObject> CreateObject(const std::string& _Name = "", Ezptr<AIMLayer> _Layer = nullptr);
	static Ezptr<AIMObject> CreatePrototype(const std::string& _Name, Ezptr<AIMScene> _Scene = nullptr);
	static Ezptr<AIMObject> CreateClone(const std::string& _Name, Ezptr<AIMScene> _Scene, Ezptr<AIMLayer> _Layer = nullptr);
	static void RemovePrototype(AIMScene* _Scene);
	static void RemovePrototype(AIMScene* _Scene, const std::string& _Prototype);

private:
	static Ezptr<AIMObject> FindPrototype(const std::string& _Name, Ezptr<AIMScene> _Scene = nullptr);

private:
	AIMScene* Scene = nullptr;
	AIMLayer* Layer = nullptr;
	Ezptr<AIMTransform> Transform = nullptr;

public:
	AIMScene* GetScene() const;
	AIMLayer* GetLayer() const;
	Ezptr<AIMTransform> GetTransform() const;

public:
	void SetScene(AIMScene* _Scene);
	void SetLayer(AIMLayer* _Layer);

private:
	std::list<Ezptr<AIMComponent>> ComList;

public:
	void Start();
	bool Init();
	int Input(float _Time);
	int Update(float _Time);
	int LateUpdate(float _Time);
	int Collision(float _Time);
	int Render(float _Time);

	AIMObject* Clone() const;

public:
	bool CheckComponent(const std::string& _Name);
	bool CheckComponent(ComType _Type);

public:
	AIMComponent* AddComponent(Ezptr<AIMComponent> _Com);

	template<typename T>
	T* AddComponent(const std::string& _Name = "")
	{
		T* Com = new T;

		Com->SetNameTag(_Name.c_str());
		Com->SetScene(Scene);
		Com->SetLayer(Layer);
		Com->SetAIMObject(this);

		if (Com->Init() == false)
		{
			return nullptr;
		}

		return (T*)AddComponent(Com);
	}

	template<typename T>
	Ezptr<T> FindComponent(const std::string& _Name)
	{
		std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
		std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

		for (; StartIter != EndIter ; ++StartIter)
		{
			if ((*StartIter)->GetNameTag() == _Name)
			{
				return (*StartIter);
			}
		}

		return nullptr;
	}

	template<typename T>
	Ezptr<T> FindComponent(ComType _Type)
	{
		std::list<Ezptr<AIMComponent>>::iterator StartIter = ComList.begin();
		std::list<Ezptr<AIMComponent>>::iterator EndIter = ComList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if ((*StartIter)->GetComType() == _Type)
			{
				return (*StartIter);
			}
		}

		return nullptr;
	}
private:
	AIMObject();
	AIMObject(const AIMObject& _Other);
	~AIMObject();
};

