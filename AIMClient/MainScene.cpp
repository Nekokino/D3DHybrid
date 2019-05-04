#include "MainScene.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMObject.h>
#include <AIMRenderer.h>

#include "Player.h"
#include "Minion.h"
#include "Bullet.h"

MainScene::MainScene()
{
}


MainScene::~MainScene()
{
	int a = 0;
}

bool MainScene::Init()
{
	Ezptr<AIMLayer> DefaultLayer = Scene->FindLayer("Default");

	Ezptr<AIMObject> PlayerObj = AIMObject::CreateObject("Player", DefaultLayer);

	Ezptr<Player> PlayerCom = PlayerObj->AddComponent<Player>("Player");

	Ezptr<AIMObject> MinionProto = AIMObject::CreatePrototype("Minion");

	Ezptr<Minion> MinionCom = MinionProto->AddComponent<Minion>("Minion");

	Ezptr<AIMObject> BulletProto = AIMObject::CreatePrototype("Bullet");
	
	Ezptr<Bullet> BulletCom = BulletProto->AddComponent<Bullet>("Bullet");

	Ezptr<AIMObject> MinionObj = AIMObject::CreateClone("Minion", Scene, DefaultLayer);

	Ezptr<AIMTransform> MinionTransform = MinionObj->GetTransform();

	MinionTransform->SetWorldPosition(1.0f, 1.0f, 0.0f);
	MinionTransform->LookAt(PlayerObj);

	return true;
}