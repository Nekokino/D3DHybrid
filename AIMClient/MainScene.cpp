#include "MainScene.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMObject.h>
#include <AIMRenderer.h>
#include <CollisionManager.h>
#include <AIMMaterial.h>
#include <AIMParticle.h>
#include <AIMFrameAnimation.h>
#include <AIMLandScape.h>
#include <AIMArm.h>

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
	CollisionManager::AddChannel("Trace");

	CollisionManager::CreateProfile("Player");
	CollisionManager::CreateProfile("Enemy");
	CollisionManager::CreateProfile("Trace");

	//CollisionManager::SetProfileChannelState("Player", "Player", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Player", "WorldStatic", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Player", "WorldDynamic", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Player", "EnemyAttack", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Player", "Enemy", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Player", "Trace", CCS_BLOCK);

	CollisionManager::SetProfileChannelState("Enemy", "PlayerAttack", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Enemy", "Player", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Enemy", "WorldStatic", CCS_BLOCK);
	CollisionManager::SetProfileChannelState("Enemy", "WorldDynamic", CCS_BLOCK);

	CollisionManager::SetProfileChannelState("Trace", "Player", CCS_BLOCK);

	Ezptr<AIMLayer> DefaultLayer = Scene->FindLayer("Default");

	Ezptr<AIMObject> MainCameraObj = Scene->GetMainCameraObj();

	Ezptr<AIMArm> CamArm = MainCameraObj->AddComponent<AIMArm>("CamArm");

	CamArm->SetMouseEnable(true);

	CreatePrototype();

	Ezptr<AIMObject> PlayerObj = AIMObject::CreateObject("Player", DefaultLayer);

	CamArm->SetTarget(PlayerObj);

	Ezptr<Player> PlayerCom = PlayerObj->AddComponent<Player>("Player");

	Ezptr<AIMObject> MinionObj = AIMObject::CreateClone("Minion", Scene, DefaultLayer);

	Ezptr<AIMTransform> MinionTransform = MinionObj->GetTransform();

	MinionTransform->SetWorldPosition(15.0f, 0.0f, 30.0f);

	PlayerObj->GetTransform()->SetWorldScale(0.05f, 0.05f, 0.05f);
	PlayerObj->GetTransform()->SetLocalRotation(0.0f, -90.0f, 0.0f);
	PlayerObj->GetTransform()->SetWorldPosition(1.0f, 0.0f, 1.0f);

	MinionObj->GetTransform()->SetWorldScale(0.05, 0.05f, 0.05f);
	MinionObj->GetTransform()->SetLocalRotation(0.0f, -90.0f, 0.0f);

	Ezptr<AIMObject> Explosion = AIMObject::CreateClone("Explosion", Scene, DefaultLayer);

	Ezptr<AIMObject> LandScapeObj = AIMObject::CreateObject("LandScape", DefaultLayer);

	Ezptr<AIMLandScape> LandScape = LandScapeObj->AddComponent<AIMLandScape>("LandScape");

	LandScape->CreateLandScape("LandScape", "LandScape/Height3.bmp");
	LandScape->AddSplattingTexture(TEXT("LandScape/BD_Terrain_Cliff05.dds"), TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"), TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"), TEXT("LandScape/RoadAlpha.bmp"));
	LandScape->AddSplattingTexture(TEXT("LandScape/Terrain_Cliff_15_Large.dds"), TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"), TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"), TEXT("LandScape/SandBaseAlpha.bmp"));
	LandScape->AddSplattingTexture(TEXT("LandScape/Terrain_Pebbles_01.dds"), TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"), TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"), TEXT("LandScape/WaterBaseAlpha.bmp"));

	LandScape->CreateSplattingTexture();

	for (int AA = 0; AA < 12; AA++)
	{
		for (int BB = 0; BB < 25; BB++)
		{
			Ezptr<AIMObject> ChairObj = AIMObject::CreateClone("Chair", Scene, DefaultLayer);

			Ezptr<AIMTransform> ChairTr = ChairObj->GetTransform();

			ChairTr->SetWorldScale(0.02f, 0.02f, 0.02f);
			ChairTr->SetWorldPosition(BB * 2.0f, 2.0f, 7.0f + AA * 2.0f);
		}
	}

	return true;
}

void MainScene::CreatePrototype()
{
	Ezptr<AIMObject> MinionProto = AIMObject::CreatePrototype("Minion", Scene);

	Ezptr<Minion> MinionCom = MinionProto->AddComponent<Minion>("Minion");

	Ezptr<AIMObject> BulletProto = AIMObject::CreatePrototype("Bullet", Scene);

	Ezptr<Bullet> BulletCom = BulletProto->AddComponent<Bullet>("Bullet");

	Ezptr<AIMObject> ExplosionProto = AIMObject::CreatePrototype("Explosion", Scene);

	Ezptr<AIMTransform> ExplosionTransform = ExplosionProto->GetTransform();

	ExplosionTransform->SetWorldScale(3.0f, 3.0f, 1.0f);

	Ezptr<AIMRenderer> ExplosionRenderer = ExplosionProto->AddComponent<AIMRenderer>("ExplosionRenderer");

	ExplosionRenderer->SetMesh("Particle");
	ExplosionRenderer->SetRenderState("AlphaBlend");

	Ezptr<AIMParticle> Particle = ExplosionProto->AddComponent<AIMParticle>("Explosion");

	std::vector<TCHAR*> ParticleFileNameVec;
	ParticleFileNameVec.reserve(89);

	for (size_t i = 0; i <= 89; i++)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("Explosion/Explosion%d.png"), i);

		ParticleFileNameVec.push_back(FileName);
	}

	Particle->LoadTextureSet("Explosion", ParticleFileNameVec);

	Ezptr<AIMFrameAnimation> ExplosionFrmAnim = ExplosionProto->AddComponent<AIMFrameAnimation>("ExplosionAnimation");

	std::vector<TextureCoord> CoordVec;
	CoordVec.reserve(89);

	for (int i = 0; i < 89; i++)
	{
		TextureCoord Coord = {};
		Coord.Start = Vec2(0.0f, 0.0f);
		Coord.End = Vec2(320.0f, 240.0f);

		CoordVec.push_back(Coord);
	}

	ExplosionFrmAnim->CreateClip("Idle", AO_LOOP, Vec2(320.f, 240.0f), CoordVec, 1.0f, 10, "Explosion", ParticleFileNameVec);

	for (size_t i = 0; i < ParticleFileNameVec.size(); i++)
	{
		if (ParticleFileNameVec[i] != nullptr)
		{
			delete[] ParticleFileNameVec[i];
			ParticleFileNameVec[i] = nullptr;
		}
	}

	Ezptr<AIMObject> ChairProto = AIMObject::CreatePrototype("Chair", Scene);

	Ezptr<AIMRenderer> ChairRenderer = ChairProto->AddComponent<AIMRenderer>("ChairRenderer");

	ChairRenderer->SetMesh("Chair", TEXT("chair.msh"));
}
