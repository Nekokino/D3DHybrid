#include "AIMColliderPoint.h"



AIMColliderPoint::AIMColliderPoint()
{
	SetNameTag("PointCol");
	ColType = CT_POINT;
}

AIMColliderPoint::AIMColliderPoint(const AIMColliderPoint & _Other) : AIMCollider(_Other)
{
}


AIMColliderPoint::~AIMColliderPoint()
{
}

void AIMColliderPoint::Start()
{
	AIMCollider::Start();
}

bool AIMColliderPoint::Init()
{
	return true;
}

int AIMColliderPoint::Input(float _Time)
{
	return 0;
}

int AIMColliderPoint::Update(float _Time)
{
	return 0;
}

int AIMColliderPoint::LateUpdate(float _Time)
{
	return 0;
}

int AIMColliderPoint::Collision(float _Time)
{
	return 0;
}

int AIMColliderPoint::PrevRender(float _Time)
{
	return 0;
}

int AIMColliderPoint::Render(float _Time)
{
	return 0;
}

AIMColliderPoint * AIMColliderPoint::Clone() const
{
	return new AIMColliderPoint(*this);
}

bool AIMColliderPoint::Collision(Ezptr<AIMCollider> _Dest)
{
	return false;
}
