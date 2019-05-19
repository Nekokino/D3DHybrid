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

Player::Player()
{
}

Player::Player(const Player & _Other)
{
}


Player::~Player()
{
}

void Player::Start()
{
}

bool Player::Init()
{

	Ezptr<AIMRenderer> Renderer = Object->AddComponent<AIMRenderer>("PlayerRenderer");

	Renderer->SetMesh("Player", TEXT("Monster4.amsh"), Vec3::Axis[AXIS_Z]);

	Ezptr<AIMMaterial> Material = Object->FindComponent<AIMMaterial>(CT_MATERIAL);

	Material->SetEmissiveColor(1.0f);

	//Ezptr<AIMColliderSphere> Col = Object->AddComponent<AIMColliderSphere>("Col");

	//Col->SetProfile("Player");
	//Col->SetChannel("Player");
	//Col->SetSphereInfo(Vec3::Zero, 1.0f);

	//Col->SetCallBack(CCS_BEGIN, this, &Player::Hit);
	//Col->SetCallBack(CCS_STAY, this, &Player::HitStay);
	//Col->SetCallBack(CCS_LEAVE, this, &Player::HitLeave);

	Ezptr<AIMColliderOBB> Col = Object->AddComponent<AIMColliderOBB>("Col");

	Col->SetProfile("Player");
	Col->SetChannel("Player");
	Col->SetOBBInfo(Vec3(0.0f, 0.5f, 0.0f), Vec3(1.0f, 0.5f, 0.5f));

	Col->SetCallBack(CCS_BEGIN, this, &Player::Hit);
	Col->SetCallBack(CCS_STAY, this, &Player::HitStay);
	Col->SetCallBack(CCS_LEAVE, this, &Player::HitLeave);

	InputManager::InputBindAxis("MoveFront", DIK_W, 10.0f, this, &Player::MoveFront);
	InputManager::InputBindAxis("MoveBack", DIK_S, -10.0f, this, &Player::MoveFront);
	InputManager::InputBindAxis("MoveFront", DIK_UP, 10.0f, this, &Player::MoveFront);
	InputManager::InputBindAxis("MoveBack", DIK_DOWN, -10.0f, this, &Player::MoveFront);

	bool SKey[SKEY_END] = { false, false, false };

	InputManager::InputBindAction("Fire", DIK_SPACE, KS_PRESS, this, &Player::Fire, SKey); // LCTRL + SPACE
	InputManager::InputBindAction("Test", DIK_SPACE, KS_PRESS, this, &Player::Test); // JUST SPACE

	Animation = Object->FindComponent<AIMAnimation>("Animation");

	if (Animation == nullptr)
	{
		Animation = Object->AddComponent<AIMAnimation>("Animation");

		Animation->LoadBone("Monster4.bne");
		Animation->Load("Monster4.anm");
	}

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
	Transform->Move(AXIS_Y, _Scale, _Time);
}

void Player::Fire(float _Time)
{
	Ezptr<AIMObject> Bullet = AIMObject::CreateClone("Bullet", Scene, Layer);

	Vec3 Pos = Transform->GetWorldPosition();
	Vec3 Rot = Transform->GetWorldRotation();
	Vec3 View = Transform->GetWorldAxis(AXIS_Y);

	Pos += View;

	Ezptr<AIMTransform> BulletTransform = Bullet->GetTransform();

	BulletTransform->SetWorldPosition(Pos);
	BulletTransform->SetWorldRotation(Rot);
}

void Player::Test(float _Time)
{
	//OutputDebugString(TEXT("Test\n"));
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
