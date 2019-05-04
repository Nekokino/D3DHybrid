#include "UserComponent.h"



UserComponent::UserComponent()
{
	CT = CT_USERCOM;
}

UserComponent::UserComponent(const UserComponent& _Other) : AIMComponent(_Other)
{

}

void UserComponent::Start()
{
}

bool UserComponent::Init()
{
	return true;
}

int UserComponent::Input(float _Time)
{
	return 0;
}

int UserComponent::Update(float _Time)
{
	return 0;
}

int UserComponent::LateUpdate(float _Time)
{
	return 0;
}

int UserComponent::Collision(float _Time)
{
	return 0;
}

int UserComponent::PrevRender(float _Time)
{
	return 0;
}

int UserComponent::Render(float _Time)
{
	return 0;
}

UserComponent * UserComponent::Clone() const
{
	return nullptr;
}
