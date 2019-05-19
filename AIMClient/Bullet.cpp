#include "Bullet.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMObject.h>
#include <AIMRenderer.h>
#include <AIMTransform.h>
#include <AIMColliderSphere.h>



Bullet::Bullet()
{
}

Bullet::Bullet(const Bullet & _Other) : UserComponent(_Other)
{
	Dist = _Other.Dist;
	Speed = _Other.Speed;
}


Bullet::~Bullet()
{
}

void Bullet::Start()
{
}

bool Bullet::Init()
{
	Ezptr<AIMRenderer> Renderer = Object->AddComponent<AIMRenderer>("BulletRenderer");

	Renderer->SetMesh("Pyramid");

	Speed = 2.0f;
	Dist = 3.0f;

	Ezptr<AIMColliderSphere> Body = Object->AddComponent<AIMColliderSphere>("Body");

	Body->SetChannel("PlayerAttack");
	Body->SetSphereInfo(Vec3::Zero, 1.0f);

	return true;
}

int Bullet::Input(float _Time)
{
	return 0;
}

int Bullet::Update(float _Time)
{
	Transform->Move(AXIS_Y, Speed, _Time);

	Dist -= Speed * _Time;

	if (Dist <= 0.0f)
	{
		Object->SetAlive(false);
	}

	return 0;
}

int Bullet::LateUpdate(float _Time)
{
	return 0;
}

int Bullet::Collision(float _Time)
{
	return 0;
}

int Bullet::PrevRender(float _Time)
{
	return 0;
}

int Bullet::Render(float _Time)
{
	return 0;
}

Bullet * Bullet::Clone() const
{
	return new Bullet(*this);
}
