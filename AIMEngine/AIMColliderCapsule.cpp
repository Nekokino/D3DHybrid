#include "AIMColliderCapsule.h"



AIMColliderCapsule::AIMColliderCapsule()
{
	SetNameTag("CapsuleCol");
	ColType = CT_CAPSULE;
}

AIMColliderCapsule::AIMColliderCapsule(const AIMColliderCapsule & _Other) : AIMCollider(_Other)
{
}


AIMColliderCapsule::~AIMColliderCapsule()
{
}

void AIMColliderCapsule::Start()
{
	AIMCollider::Start();
}

bool AIMColliderCapsule::Init()
{
	return true;
}

int AIMColliderCapsule::Input(float _Time)
{
	return 0;
}

int AIMColliderCapsule::Update(float _Time)
{
	return 0;
}

int AIMColliderCapsule::LateUpdate(float _Time)
{
	return 0;
}

int AIMColliderCapsule::Collision(float _Time)
{
	return 0;
}

int AIMColliderCapsule::PrevRender(float _Time)
{
	return 0;
}

int AIMColliderCapsule::Render(float _Time)
{
	return 0;
}

AIMColliderCapsule * AIMColliderCapsule::Clone() const
{
	return new AIMColliderCapsule(*this);
}

bool AIMColliderCapsule::Collision(Ezptr<AIMCollider> _Dest)
{
	return false;
}
