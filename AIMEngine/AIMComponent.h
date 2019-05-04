#pragma once

#include "RefCounter.h"

class AIMScene;
class AIMLayer;
class AIMObject;
class AIMTransform;
class Engine_DLL AIMComponent : public RefCounter
{
	friend class AIMObject;

protected:
	AIMScene* Scene = nullptr;
	AIMLayer* Layer = nullptr;
	AIMObject* Object = nullptr;
	AIMTransform* Transform = nullptr;

public:
	AIMScene* GetScene() const;
	AIMLayer* GetLayer() const;
	AIMObject* GetAIMObject() const;
	AIMTransform* GetTransform() const;

public:
	void SetScene(AIMScene* _Scene);
	void SetLayer(AIMLayer* _Layer);
	void SetAIMObject(AIMObject* _Object);

protected:
	ComType CT = ComType::CT_END;

public:
	ComType GetComType() const;

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMComponent* Clone() const;

public:
	template<typename T>
	Ezptr<T> FindComponent(const std::string& _Name)
	{
		return Object->FindComponent<T>(_Name);
	}

	template<typename T>
	Ezptr<T> FindComponent(ComType _Type)
	{
		return Object->FindComponent<T>(_Type);
	}

protected:
	AIMComponent();
	AIMComponent(const AIMComponent& _Other);
	virtual ~AIMComponent();
};

