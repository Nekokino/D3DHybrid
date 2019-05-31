#include "StartScene.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMObject.h>
#include <AIMRenderer.h>
#include <AIMMaterial.h>
#include <AIMParticle.h>
#include <AIMFrameAnimation.h>
#include <AIMUIButton.h>
#include <AIMLandScape.h>
#include <CollisionManager.h>
#include <SceneManager.h>
#include <AIMText.h>
#include <AIMSound.h>
#include "MainScene.h"
#include "Player.h"
#include "Minion.h"
#include "Bullet.h"


bool StartScene::Init()
{
	Ezptr<AIMLayer> DefaultLayer = Scene->FindLayer("Default");
	Ezptr<AIMLayer> UILayer = Scene->FindLayer("UI");

	Ezptr<AIMObject> StartButtonObj = AIMObject::CreateObject("StartButton", UILayer);

	Ezptr<AIMTransform> Transform = StartButtonObj->GetTransform();

	Transform->SetWorldPosition(1280.0f / 2.0f - 100.0f, 720.0f / 2.0f - 50.0f, 10.0f);

	Ezptr<AIMUIButton> StartButton = StartButtonObj->AddComponent<AIMUIButton>("StartButton");

	StartButton->SetStateTexture(BS_NORMAL, "StartButton", TEXT("Start.png"));

	StartButton->SetCallBack(BS_CLICK, this, &StartScene::StartButton);

	Ezptr<AIMObject> BGMObj = AIMObject::CreateObject("BGM", DefaultLayer);

	Ezptr<AIMSound> BGM = BGMObj->AddComponent<AIMSound>("BGM");

	BGM->SetSound("StartBGM", Scene, true, "MainBgm.mp3");
	BGM->Play();

	return true;
}

void StartScene::CreatePrototype()
{
}

void StartScene::StartButton(float _Time)
{
	Ezptr<AIMScene> Scene = SceneManager::CreateScene<MainScene>();

	SceneManager::SetNextScene(Scene);
}

StartScene::StartScene()
{
	
}


StartScene::~StartScene()
{
}
