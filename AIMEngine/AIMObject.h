#pragma once

#include "RefCounter.h"
#include "AIMComponent.h"
#include "AIMTransform.h"

class AIMLayer;
class AIMScene;
class Engine_DLL AIMObject : public RefCounter
{
public:
	static Ezptr<AIMObject> CreateObject(const std::string& _Name = "", Ezptr<AIMLayer> _Layer = nullptr);

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
	int PrevRender(float _Time);
	int Render(float _Time);

	AIMObject* Clone() const;

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
private:
	AIMObject();
	AIMObject(const AIMObject& _Other);
	~AIMObject();
};

