#include "MainScene.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMObject.h>
#include <AIMRenderer.h>

#include "Player.h"

MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

bool MainScene::Init()
{
	Ezptr<AIMLayer> DefaultLayer = Scene->FindLayer("Default");

	Ezptr<AIMObject> PlayerObj = AIMObject::CreateObject("Player", DefaultLayer);

	Ezptr<Player> PlayerCom = PlayerObj->AddComponent<Player>("Player");

	return true;
}