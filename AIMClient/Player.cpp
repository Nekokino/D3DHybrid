#include "Player.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMRenderer.h>
#include <AIMObject.h>


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

	Renderer->SetMesh("ColorTriangle");

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
