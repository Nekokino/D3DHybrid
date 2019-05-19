#include "MainScene.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMObject.h>
#include <AIMRenderer.h>
#include <CollisionManager.h>
#include <AIMMaterial.h>
#include <AIMParticle.h>

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
	CollisionManager::AddChannel("Player");
	CollisionManager::AddChannel("Enemy");
	CollisionManager::AddChannel("PlayerAttack");
	CollisionManager::AddChannel("EnemyAttack");

	CollisionManager::CreateProfile("Player");
	CollisionManager::CreateProfile("Enemy");

	//CollisionManager::SetProfileChannelState("Player", "Player", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Player", "WorldStatic", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Player", "WorldDynamic", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Player", "EnemyAttack", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Player", "Enemy", CCS_BLOCK);

	CollisionManager::SetProfileChannelState("Enemy", "PlayerAttack", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Enemy", "Player", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Enemy", "WorldStatic", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Enemy", "WorldDynamic", CCS_BLOCK);

	Ezptr<AIMLayer> DefaultLayer = Scene->FindLayer("Default");

	CreatePrototype();

	Ezptr<AIMObject> PlayerObj = AIMObject::CreateObject("Player", DefaultLayer);

	Ezptr<Player> PlayerCom = PlayerObj->AddComponent<Player>("Player");

	Ezptr<AIMObject> MinionObj = AIMObject::CreateClone("Minion", Scene, DefaultLayer);

	Ezptr<AIMTransform> MinionTransform = MinionObj->GetTransform();

	MinionTransform->SetWorldPosition(1.0f, 1.0f, 0.0f);
	MinionTransform->LookAt(PlayerObj);

	PlayerObj->GetTransform()->SetWorldScale(0.05f, 0.05f, 0.05f);

	Ezptr<AIMObject> Explosion = AIMObject::CreateClone("Explosion", Scene, DefaultLayer);

	return true;
}

void MainScene::CreatePrototype()
{
	Ezptr<AIMObject> MinionProto = AIMObject::CreatePrototype("Minion");

	Ezptr<Minion> MinionCom = MinionProto->AddComponent<Minion>("Minion");

	Ezptr<AIMObject> BulletProto = AIMObject::CreatePrototype("Bullet");

	Ezptr<Bullet> BulletCom = BulletProto->AddComponent<Bullet>("Bullet");

	Ezptr<AIMObject> ExplosionProto = AIMObject::CreatePrototype("Explosion");

	Ezptr<AIMTransform> ExplosionTransform = ExplosionProto->GetTransform();

	ExplosionTransform->SetWorldScale(3.0f, 3.0f, 1.0f);

	Ezptr<AIMRenderer> ExplosionRenderer = ExplosionProto->AddComponent<AIMRenderer>("ExplosionRenderer");

	ExplosionRenderer->SetMesh("Particle");

	Ezptr<AIMParticle> Particle = ExplosionProto->AddComponent<AIMParticle>("Explosion");
}
