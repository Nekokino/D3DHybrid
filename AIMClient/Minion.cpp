#include "Minion.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMObject.h>
#include <AIMRenderer.h>
#include <InputManager.h>
#include <AIMTransform.h>
#include <AIMColliderSphere.h>
#include <AIMColliderOBB.h>

Minion::Minion()
{
}

Minion::Minion(const Minion & _Other) : UserComponent(_Other)
{
}


Minion::~Minion()
{
	int a = 0;
}

void Minion::Start()
{
}

bool Minion::Init()
{
	Ezptr<AIMRenderer> Renderer = Object->AddComponent<AIMRenderer>("MinionRenderer");

	Renderer->SetMesh("Pyramid");

	//Ezptr<AIMColliderSphere> Body = Object->AddComponent<AIMColliderSphere>("Body");

	//Body->SetProfile("Enemy");
	//Body->SetChannel("Enemy");
	//Body->SetSphereInfo(Vec3::Zero, 1.0f);

	Ezptr<AIMColliderOBB> Body = Object->AddComponent<AIMColliderOBB>("Body");

	Body->SetProfile("Enemy");
	Body->SetChannel("Enemy");
	Body->SetOBBInfo(Vec3::Zero, Vec3(0.5f, 0.5f, 0.5f));

	return true;
}

int Minion::Input(float _Time)
{
	return 0;
}

int Minion::Update(float _Time)
{
	int a = 0;

	return 0;
}

int Minion::LateUpdate(float _Time)
{
	return 0;
}

int Minion::Collision(float _Time)
{
	return 0;
}

int Minion::PrevRender(float _Time)
{
	return 0;
}

int Minion::Render(float _Time)
{
	return 0;
}

Minion * Minion::Clone() const
{
	return new Minion(*this);
}
