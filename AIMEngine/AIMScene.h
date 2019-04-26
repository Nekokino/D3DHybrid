#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"
#include "AIMLayer.h"
#include "SceneComponent.h"

class Engine_DLL AIMScene : public RefCounter
{
	friend class SceneManager;

private:
	std::list<Ezptr<SceneComponent>> SceneComList;
	std::list<Ezptr<AIMLayer>> LayerList;

public:
	bool Init();
	int Input(float _Time);
	int Update(float _Time);
	int LateUpdate(float _Time);
	int Collision(float _Time);
	int PrevRender(float _Time);
	int Render(float _Time);

	void AddLayer(const std::string& _Name, int _Order);
	void SortLayer();

	Ezptr<AIMLayer> FindLayer(const std::string& _Name);

private:
	static bool SortFunc(Ezptr<AIMLayer> _Src, Ezptr<AIMLayer> _Dest);

public:
	template<typename T>
	bool AddSceneComponent()
	{
		T* Com = new T;

		Com->Scene = this;

		if (Com->Init() == false)
		{
			return false;
		}

		// DLLȯ�濡���� DLL�� ����ϴ��ʿ��� ����� Ŭ������ ������µ� ������ ����°� ���⵵ �ϰ�.
		// �׷��� ����Ʈ�����͸� ����ϸ� ���߻��� ����.
		// �׷� ��ؾ��ϳ� ����.
		// ezptr�� dllexport�� ������ �� �ȴ�.
		// dllexport�� �ϴ����� ����

		SceneComList.push_back(Com);

		return true;
	}

private:
	AIMScene();
	~AIMScene();
};

