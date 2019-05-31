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
	Ezptr<AIMObject> UICameraObj;
	Ezptr<AIMCamera> UICamera;
	Ezptr<AIMTransform> UICameraTransform;

	Ezptr<AIMObject> Sky;

public:
	Ezptr<AIMObject> GetMainCameraObj() const;
	Ezptr<AIMCamera> GetMainCamera() const;
	Ezptr<AIMTransform> GetMainCameraTransform() const;
	Ezptr<AIMObject> GetUICameraObj() const;
	Ezptr<AIMCamera> GetUICamera() const;
	Ezptr<AIMTransform> GetUICameraTransform() const;
	Matrix GetViewMatrix() const;
	Matrix GetProjectionMatrix() const;
	Vec3 GetMainCameraPos() const;
	Matrix GetUICamViewMatrix() const;
	Matrix GetUICamProjectionMatrix() const;
	Vec3 GetUICameraPos() const;

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

		// DLL환경에서는 DLL을 사용하는쪽에서 사용자 클래스를 끌어오는데 문제가 생기는것 같기도 하고.
		// 그래서 스마트포인터를 사용하면 개발살이 난다.
		// 그럼 어떡해야하냐 에휴.
		// ezptr을 dllexport를 껐더니 잘 된다.
		// dllexport가 하는일이 뭐지

		SceneComList.push_back(Com);

		return true;
	}

private:
	AIMScene();
	~AIMScene();
};

