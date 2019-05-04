#include "AIMLight.h"
#include "ShaderManager.h"
#include "AIMTransform.h"
#include "AIMScene.h"
#include "AIMCamera.h"

AIMLight::AIMLight()
{
	CT = CT_LIGHT;
}

AIMLight::AIMLight(const AIMLight & _Other) : AIMComponent(_Other)
{
	Info = _Other.Info;
}


AIMLight::~AIMLight()
{
}

void AIMLight::SetShader()
{
	ShaderManager::UpdateConstBuffer("Light", &Info);
}

void AIMLight::Start()
{
}

bool AIMLight::Init()
{
	return true;
}

int AIMLight::Input(float _Time)
{
	return 0;
}

int AIMLight::Update(float _Time)
{
	return 0;
}

int AIMLight::LateUpdate(float _Time)
{
	Info.Pos = Transform->GetWorldPosition();
	Info.Direction = Transform->GetWorldAxis(AXIS_Z);

	Ezptr<AIMCamera> Cam = Scene->GetMainCamera();

	Info.Pos = Info.Pos.TransformCoord(Cam->GetView());
	Info.Direction = Info.Direction.TransformNormal(Cam->GetView()) * -1.0f;
	Info.Direction.Normalize();

	return 0;
}

int AIMLight::Collision(float _Time)
{
	return 0;
}

int AIMLight::PrevRender(float _Time)
{
	return 0;
}

int AIMLight::Render(float _Time)
{
	return 0;
}

AIMLight * AIMLight::Clone() const
{
	return new AIMLight(*this);
}
