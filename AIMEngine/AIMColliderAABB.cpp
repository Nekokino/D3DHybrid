#include "AIMColliderAABB.h"



AIMColliderAABB::AIMColliderAABB()
{
	SetNameTag("AABBCol");
	ColType = CT_AABB;
}

AIMColliderAABB::AIMColliderAABB(const AIMColliderAABB & _Other) : AIMCollider(_Other)
{

}


AIMColliderAABB::~AIMColliderAABB()
{
}

void AIMColliderAABB::Start()
{
	AIMCollider::Start();
}

bool AIMColliderAABB::Init()
{
	return true;
}

int AIMColliderAABB::Input(float _Time)
{
	return 0;
}

int AIMColliderAABB::Update(float _Time)
{
	return 0;
}

int AIMColliderAABB::LateUpdate(float _Time)
{
	return 0;
}

int AIMColliderAABB::Collision(float _Time)
{
	return 0;
}

int AIMColliderAABB::PrevRender(float _Time)
{
	return 0;
}

int AIMColliderAABB::Render(float _Time)
{
	return 0;
}

AIMColliderAABB * AIMColliderAABB::Clone() const
{
	return new AIMColliderAABB(*this);
}

bool AIMColliderAABB::Collision(Ezptr<AIMCollider> _Dest)
{
	return false;
}
