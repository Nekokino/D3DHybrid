#include "Player.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMRenderer.h>
#include <AIMObject.h>
#include <InputManager.h>
#include <AIMTransform.h>
#include <AIMColliderSphere.h>
#include <AIMColliderOBB.h>
#include <AIMMaterial.h>
#include <NavigationManager.h>
#include <NavigationMesh.h>
#include <AIMNavigation.h>

Player::Player()
{
}

Player::Player(const Player & _Other) : UserComponent(_Other)
{
	Animation = _Other.Animation;
}


Player::~Player()
{
}

void Player::Start()
{
	Ezptr<AIMCollider> PickSphere = FindComponent<AIMCollider>("PickSphere");

	PickSphere->SetCallBack<Player>(CCS_STAY, this, &Player::HitPick);
	PickSphere->SetCallBack<Player>(CCS_LEAVE, this, &Player::HitPickLeave);
}

bool Player::Init()
{

	Ezptr<AIMRenderer> Renderer = Object->AddComponent<AIMRenderer>("PlayerRenderer");

	Renderer->SetMesh("Player", TEXT("Monster4.amsh"), Vec3::Axis[AXIS_Z]);

	Ezptr<AIMMaterial> Material = Object->FindComponent<AIMMaterial>(CT_MATERIAL);

	//Material->SetEmissiveColor(1.0f);

	Ezptr<AIMColliderSphere> Col = Object->AddComponent<AIMColliderSphere>("PlayerBody");

	Col->SetProfile("Player");
	Col->SetChannel("Player");
	Col->SetSphereInfo(Vec3::Zero, 1.0f);

	//Col->SetCallBack(CCS_BEGIN, this, &Player::Hit);
	//Col->SetCallBack(CCS_STAY, this, &Player::HitStay);
	//Col->SetCallBack(CCS_LEAVE, this, &Player::HitLeave);
	Col->SetScaleEnable(false);

	//Ezptr<AIMColliderOBB> Col = Object->AddComponent<AIMColliderOBB>("Col");

	//Col->SetProfile("Player");
	//Col->SetChannel("Player");
	//Col->SetOBBInfo(Vec3(0.0f, 0.5f, 0.0f), Vec3(1.0f, 0.5f, 0.5f));

	//Col->SetCallBack(CCS_BEGIN, this, &Player::Hit);
	//Col->SetCallBack(CCS_STAY, this, &Player::HitStay);
	//Col->SetCallBack(CCS_LEAVE, this, &Player::HitLeave);

	InputManager::AddAxisKey("Move", DIK_W, 1.0f);
	InputManager::AddAxisKey("Move", DIK_S, -1.0f);
	InputManager::AddAxisKey("Move", DIK_UP, 1.0f);
	InputManager::AddAxisKey("Move", DIK_DOWN, -1.0f);
	InputManager::BindAxis("Move", this, &Player::MoveFront);

	InputManager::AddAxisKey("Rotation", DIK_A, -1.0f);
	InputManager::AddAxisKey("Rotation", DIK_D, 1.0f);
	InputManager::BindAxis("Rotation", this, &Player::Rotation);

	InputManager::AddActionKey("Fire", DIK_SPACE);
	InputManager::BindAction("Fire", KS_PRESS, this, &Player::Fire);

	InputManager::AddActionKey("Test", DIK_SPACE, SE_CTRL);
	InputManager::BindAction("Test", KS_RELEASE, this, &Player::Test);


	bool SKey[SKEY_END] = { false, false, false };

	Animation = Object->FindComponent<AIMAnimation>("Animation");

	if (Animation == nullptr)
	{
		Animation = Object->AddComponent<AIMAnimation>("Animation");

		Animation->LoadBone("Monster4.bne");
		Animation->Load("Monster4.anm");
	}

	Ezptr<AIMNavigation> Navi = Object->AddComponent<AIMNavigation>("Navi");

	return true;
}

int Player::Input(float _Time)
{
	return 0;
}

int Player::Update(float _Time)
{
	return 0;
}

int Player::LateUpdate(float _Time)
{
	return 0;
}

int Player::Collision(float _Time)
{
	return 0;
}

int Player::PrevRender(float _Time)
{
	return 0;
}

int Player::Render(float _Time)
{
	return 0;
}

Player * Player::Clone() const
{
	return new Player(*this);
}

void Player::MoveFront(float _Scale, float _Time)
{
	Transform->Move(AXIS_Z, _Scale * 5.0f, _Time);
}

void Player::Rotation(float _Scale, float _Time)
{
	Transform->RotationY(180.0f * _Scale, _Time);
}

void Player::Fire(float _Time)
{
	Ezptr<AIMObject> Bullet = AIMObject::CreateClone("Bullet", Scene, Layer);

	Vec3 Pos = Transform->GetWorldPosition();
	Vec3 Rot = Transform->GetWorldRotation();
	Vec3 View = Transform->GetWorldAxis(AXIS_Z);

	Pos += View;

	Ezptr<AIMTransform> BulletTransform = Bullet->GetTransform();

	BulletTransform->SetWorldPosition(Pos);
	BulletTransform->SetWorldRotation(Rot);
}

void Player::Test(float _Time)
{
	Ezptr<AIMObject> Bullet = AIMObject::CreateClone("Bullet", Scene, Layer);

	Vec3 Pos = Transform->GetWorldPosition();
	Vec3 Rot = Transform->GetWorldRotation();
	Vec3 View = Transform->GetWorldAxis(AXIS_Z);

	Pos += View;

	Ezptr<AIMTransform> BulletTransform = Bullet->GetTransform();

	BulletTransform->SetWorldPosition(Pos);
	BulletTransform->SetWorldRotation(Rot);
}

void Player::Hit(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest, float _Time)
{
	OutputDebugString(TEXT("충돌시작\n"));
}

void Player::HitStay(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest, float _Time)
{
	OutputDebugString(TEXT("충돌중\n"));
}

void Player::HitLeave(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest, float _Time)
{
	OutputDebugString(TEXT("충돌나감\n"));
}

void Player::HitPick(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest, float _Time)
{
	Ezptr<AIMMaterial> Material = FindComponent<AIMMaterial>(CT_MATERIAL);

	Material->SetEmissiveColor(1.0f);
}

void Player::HitPickLeave(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest, float _Time)
{
	Ezptr<AIMMaterial> Material = FindComponent<AIMMaterial>(CT_MATERIAL);

	Material->SetEmissiveColor(0.0f);
}
