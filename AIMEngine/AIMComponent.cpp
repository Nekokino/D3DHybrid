#include "AIMComponent.h"
#include "AIMObject.h"
#include "AIMTransform.h"
AIMComponent::AIMComponent()
{
}

AIMComponent::AIMComponent(const AIMComponent & _Other) : Scene(_Other.Scene), Layer(_Other.Layer), Object(_Other.Object), Transform(_Other.Transform), CT(_Other.CT), RefCounter(_Other)
{

}

AIMComponent::~AIMComponent()
{
}

AIMScene * AIMComponent::GetScene() const
{
	return Scene;
}

AIMLayer * AIMComponent::GetLayer() const
{
	return Layer;
}

AIMObject * AIMComponent::GetAIMObject() const
{
	return Object;
}

AIMTransform * AIMComponent::GetTransform() const
{
	return Transform;
}

bool AIMComponent::IsObjectEnable()
{
	return Object->IsEnable();
}

bool AIMComponent::IsObjectAlive()
{
	return Object->IsAlive();
}

void AIMComponent::SetScene(AIMScene * _Scene)
{
	Scene = _Scene;
}

void AIMComponent::SetLayer(AIMLayer * _Layer)
{
	Layer = _Layer;
}

void AIMComponent::SetAIMObject(AIMObject * _Object)
{
	Object = _Object;
}

ComType AIMComponent::GetComType() const
{
	return CT;
}

void AIMComponent::Start()
{
}

bool AIMComponent::Init()
{
	return true;
}

int AIMComponent::Input(float _Time)
{
	return 0;
}

int AIMComponent::Update(float _Time)
{
	return 0;
}

int AIMComponent::LateUpdate(float _Time)
{
	return 0;
}

int AIMComponent::Collision(float _Time)
{
	return 0;
}

int AIMComponent::PrevRender(float _Time)
{
	return 0;
}

int AIMComponent::Render(float _Time)
{
	return 0;
}

AIMComponent* AIMComponent::Clone() const
{
	return new AIMComponent(*this);
}

void AIMComponent::EraseComponent(const std::string & _Name)
{
	Object->EraseComponent(_Name);
}

void AIMComponent::EraseComponent(ComType _Type)
{
	Object->EraseComponent(_Type);
}

void AIMComponent::EraseComponent(Ezptr<AIMComponent> _Com)
{
	Object->EraseComponent(_Com);
}
