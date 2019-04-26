#include "SceneManager.h"
#include "AIMScene.h"

Ezptr<AIMScene> SceneManager::Scene;
Ezptr<AIMScene> SceneManager::NextScene;


bool SceneManager::Init()
{
	Scene = CreateEmptyScene();

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

	return true;
}

int SceneManager::Update(float _Time)
{
	Scene->Update(_Time);

	return 0;
}

int SceneManager::LateUpdate(float _Time)
{
	Scene->LateUpdate(_Time);

	return 0;
}

int SceneManager::Collision(float _Time)
{
	Scene->Collision(_Time);

	return 0;
}

int SceneManager::PrevRender(float _Time)
{
	Scene->PrevRender(_Time);

	return 0;
}

int SceneManager::Render(float _Time)
{
	Scene->Render(_Time);

	return 0;
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
