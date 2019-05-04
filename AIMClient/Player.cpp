#include "Player.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMRenderer.h>
#include <AIMObject.h>
#include <InputManager.h>
#include <AIMTransform.h>


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

	Renderer->SetMesh("Pyramid");

	InputManager::InputBindAxis("MoveFront", DIK_W, 10.0f, this, &Player::MoveFront);
	InputManager::InputBindAxis("MoveBack", DIK_S, -10.0f, this, &Player::MoveFront);
	InputManager::InputBindAxis("MoveFront", DIK_UP, 10.0f, this, &Player::MoveFront);
	InputManager::InputBindAxis("MoveBack", DIK_DOWN, -10.0f, this, &Player::MoveFront);

	bool SKey[SKEY_END] = { false, false, false };

	InputManager::InputBindAction("Fire", DIK_SPACE, KS_PRESS, this, &Player::Fire, SKey); // LCTRL + SPACE
	InputManager::InputBindAction("Test", DIK_SPACE, KS_PRESS, this, &Player::Test); // JUST SPACE

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
	OutputDebugString(TEXT("Test\n"));
}
