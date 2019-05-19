#include "AIMColliderRay.h"



AIMColliderRay::AIMColliderRay()
{
	SetNameTag("RayCol");
	ColType = CT_RAY;
}

AIMColliderRay::AIMColliderRay(const AIMColliderRay & _Other) : AIMCollider(_Other)
{
}


AIMColliderRay::~AIMColliderRay()
{
}

void AIMColliderRay::Start()
{
	AIMCollider::Start();
}

bool AIMColliderRay::Init()
{
	return true;
}

int AIMColliderRay::Input(float _Time)
{
	return 0;
}

int AIMColliderRay::Update(float _Time)
{
	return 0;
}

int AIMColliderRay::LateUpdate(float _Time)
{
	return 0;
}

int AIMColliderRay::Collision(float _Time)
{
	return 0;
}

int AIMColliderRay::PrevRender(float _Time)
{
	return 0;
}

int AIMColliderRay::Render(float _Time)
{
	return 0;
}

AIMColliderRay * AIMColliderRay::Clone() const
{
	return new AIMColliderRay(*this);
}

bool AIMColliderRay::Collision(Ezptr<AIMCollider> _Dest)
{
	return false;
}
