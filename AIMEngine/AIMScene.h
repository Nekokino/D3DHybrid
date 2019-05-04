#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"
#include "AIMLayer.h"
#include "AIMCamera.h"
#include "SceneComponent.h"
#include "AIMMaterial.h"


class Engine_DLL AIMScene : public RefCounter
{
	friend class SceneManager;

private:
	std::list<Ezptr<SceneComponent>> SceneComList;
	std::list<Ezptr<AIMLayer>> LayerList;
	std::unordered_map<std::string, Ezptr<AIMObject>> CameraMap;
	Ezptr<AIMObject> MainCameraObj;
	Ezptr<AIMCamera> MainCamera;
	Ezptr<AIMTransform> MainCameraTransform;

	Ezptr<AIMObject> Sky;

public:
	Ezptr<AIMObject> GetMainCameraObj() const;
	Ezptr<AIMCamera> GetMainCamera() const;
	Ezptr<AIMTransform> GetMainCameraTransform() const;

public:
	bool Init();
	int Input(float _Time);
	int Update(float _Time);
	int LateUpdate(float _Time);
	int Collision(float _Time);
	int Render(float _Time);

	void AddLayer(const std::string& _Name, int _Order);
	void SortLayer();

	Ezptr<AIMLayer> FindLayer(const std::string& _Name);

	bool AddCamera(const std::string& _Name, const Vec3& _Pos, const Vec3& _Rot, CamType _Type, UINT _Widht, UINT _Height, float _Angle, float _Near, float _Far);
	bool ChangeCamera(const std::string& _Name);
	Ezptr<AIMObject> FindCamera(const std::string& _Name);

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

