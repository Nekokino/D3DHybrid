#include "AIMUI.h"
#include "AIMTransform.h"
#include "AIMObject.h"


AIMUI::AIMUI()
{
	CT = CT_UI;
}

AIMUI::AIMUI(const AIMUI & _Other) : AIMComponent(_Other)
{
}


AIMUI::~AIMUI()
{
}

void AIMUI::Start()
{
}

bool AIMUI::Init()
{
	Transform->UIEnable();
	Object->SetRenderGroup(RG_UI);
	return true;
}

int AIMUI::Input(float _Time)
{
	return 0;
}

int AIMUI::Update(float _Time)
{
	return 0;
}

int AIMUI::LateUpdate(float _Time)
{
	return 0;
}

int AIMUI::Collision(float _Time)
{
	return 0;
}

int AIMUI::PrevRender(float _Time)
{
	return 0;
}

int AIMUI::Render(float _Time)
{
	return 0;
}

AIMUI * AIMUI::Clone() const
{
	return new AIMUI(*this);
}
