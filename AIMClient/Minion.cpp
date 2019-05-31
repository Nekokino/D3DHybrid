#include "Minion.h"
#include <AIMScene.h>
#include <AIMLayer.h>
#include <AIMObject.h>
#include <AIMRenderer.h>
#include <InputManager.h>
#include <AIMTransform.h>
#include <AIMColliderSphere.h>
#include <AIMColliderOBB.h>
#include <AIMMaterial.h>
#include <AIMAnimation.h>
#include <AIMNavigation.h>

Minion::Minion()
{
}

Minion::Minion(const Minion & _Other) : UserComponent(_Other)
{
}


Minion::~Minion()
{
}

void Minion::Trace(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest, float _Time)
{
	if (Navigation == nullptr)
	{
		Navigation = Object->FindComponent<AIMNavigation>(CT_NAVIGATION);
	}

	if (_Dest->GetNameTag() == "PlayerBody")
	{
		Navigation->SetTarget(_Dest->GetAIMObject());
		Navigation->SetAIFindPath(true);
	}
}

void Minion::TraceExit(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest, float _Time)
{
	if (Navigation == nullptr)
	{
		Navigation = Object->FindComponent<AIMNavigation>(CT_NAVIGATION);
	}

	if (_Dest->GetNameTag() == "PlayerBody")
	{
		Navigation->SetTarget(nullptr);
		Navigation->SetAIFindPath(false);
	}
}

void Minion::Start()
{
	Animation = Object->FindComponent<AIMAnimation>(CT_ANIMATION);

	Animation->ChangeClip("Attack");

	Navigation = Object->FindComponent<AIMNavigation>(CT_NAVIGATION);

	Ezptr<AIMColliderSphere> Trace = Object->FindComponent<AIMColliderSphere>("Trace");

	Trace->SetCallBack<Minion>(CCS_BEGIN, this, &Minion::Trace);
	Trace->SetCallBack<Minion>(CCS_LEAVE, this, &Minion::TraceExit);
	//Trace->SetScaleEnable(false);
}

bool Minion::Init()
{
	Ezptr<AIMRenderer> Renderer = Object->AddComponent<AIMRenderer>("MinionRenderer");

	Renderer->SetMesh("Player", TEXT("Monster4.amsh"), Vec3::Axis[AXIS_Z]);

	Ezptr<AIMMaterial> Material = Object->FindComponent<AIMMaterial>(CT_MATERIAL);

	Ezptr<AIMColliderOBB> Body = Object->AddComponent<AIMColliderOBB>("Body");

	Body->SetProfile("Enemy");
	Body->SetChannel("Enemy");
	Body->SetOBBInfo(Vec3::Zero, Vec3(0.5f, 0.5f, 0.5f));

	Ezptr<AIMColliderSphere> Trace = Object->AddComponent<AIMColliderSphere>("Trace");

	Trace->SetProfile("Trace");
	Trace->SetChannel("Trace");
	Trace->SetSphereInfo(Vec3::Zero, 15.0f);
	Trace->SetScaleEnable(false);

	Animation = Object->FindComponent<AIMAnimation>(CT_ANIMATION);

	if (Animation == nullptr)
	{
		Animation = Object->AddComponent<AIMAnimation>("MinionAnimation");

		Animation->LoadBone("Monster4.bne");
		Animation->Load("Monster4.anm");
	}

	Ezptr<AIMNavigation> Navi = Object->AddComponent<AIMNavigation>("Navi");

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
