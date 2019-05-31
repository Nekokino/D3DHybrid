#include "SceneManager.h"
#include "AIMScene.h"
#include "InputManager.h"

Ezptr<AIMScene> SceneManager::Scene;
Ezptr<AIMScene> SceneManager::NextScene;


void SceneManager::SetNextScene(Ezptr<AIMScene> _Scene)
{
	NextScene = _Scene;
}

int SceneManager::ChangeScene()
{
	if (NextScene != nullptr)
	{
		Scene = NextScene;
		NextScene = nullptr;

		InputManager::ChangeMouseScene(Scene);

		return SC_NEXT;
	}

	return SC_NONE;
}

bool SceneManager::Init()
{
	Scene = CreateEmptyScene();

	InputManager::ChangeMouseScene(Scene);

	return true;
}

void SceneManager::Release()
{
	Scene = nullptr;
	NextScene = nullptr;
}

int SceneManager::Input(float _Time)
{
	Scene->Input(_Time);

	return ChangeScene();
}

int SceneManager::Update(float _Time)
{
	Scene->Update(_Time);

	return ChangeScene();
}

int SceneManager::LateUpdate(float _Time)
{
	Scene->LateUpdate(_Time);

	return ChangeScene();
}

int SceneManager::Collision(float _Time)
{
	Scene->Collision(_Time);

	return ChangeScene();
}

int SceneManager::Render(float _Time)
{
	Scene->Render(_Time);

	return ChangeScene();
}

Ezptr<AIMScene> SceneManager::GetScene()
{
	return Scene;
}

Matrix SceneManager::GetViewMatrix()
{
	return Scene->GetViewMatrix();
}

Matrix SceneManager::GetProjectionMatrix()
{
	return Scene->GetProjectionMatrix();
}

Vec3 SceneManager::GetMainCameraPos()
{
	return Scene->GetMainCameraPos();
}

Ezptr<AIMScene> SceneManager::CreateEmptyScene()
{
	Ezptr<AIMScene> Tmp = new AIMScene;

	if (Tmp->Init() == false)
	{
		return nullptr;
	}

	return Tmp;
}
