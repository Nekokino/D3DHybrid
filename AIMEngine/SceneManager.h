#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"

class AIMScene;
class Engine_DLL SceneManager
{
private:
	static Ezptr<AIMScene> Scene;
	static Ezptr<AIMScene> NextScene;

public:
	static bool Init();
	static void Release();
	static int Input(float _Time);
	static int Update(float _Time);
	static int LateUpdate(float _Time);
	static int Collision(float _Time);
	static int PrevRender(float _Time);
	static int Render(float _Time);

public:
	template<typename T>
	static Ezptr<AIMScene> CreateScene()
	{
		AIMScene* NewScene = new AIMScene;

		if (NewScene->Init() == false)
		{
			return nullptr;
		}

		NewScene->AddSceneComponent<T>();

		return NewScene;
	}

	template<typename T>
	static bool AddSceneComponent(bool _Current = true)
	{
		if (_Current == true)
		{
			return Scene->AddSceneComponent<T>();
		}

		return NextScene->AddSceneComponent<T>();
	}

	static Ezptr<AIMScene> CreateEmptyScene();

private:
	SceneManager() {}
	virtual ~SceneManager() = 0 {}
};

