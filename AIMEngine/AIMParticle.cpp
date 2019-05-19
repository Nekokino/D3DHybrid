#include "AIMParticle.h"
#include "ShaderManager.h"
#include "AIMTransform.h"
#include "AIMScene.h"
#include "AIMObject.h"


AIMParticle::AIMParticle()
{
}

AIMParticle::AIMParticle(const AIMParticle & _Other) : AIMComponent(_Other)
{
	CBuffer = _Other.CBuffer;
}


AIMParticle::~AIMParticle()
{
}

bool AIMParticle::LoadTexture(const std::string & _Name, const std::vector<TCHAR*>& _FileNameVec, const std::string & _Path)
{
	return true;
}

bool AIMParticle::LoadTexture(const std::string & _Name, const std::vector<TCHAR*>& _FullPathVec)
{
	return true;
}

void AIMParticle::Start()
{
}

bool AIMParticle::Init()
{
	Object->SetRenderGroup(RG_ALPHA);

	return true;
}

int AIMParticle::Input(float _Time)
{
	return 0;
}

int AIMParticle::Update(float _Time)
{
	return 0;
}

int AIMParticle::LateUpdate(float _Time)
{
	CBuffer.Pos = Transform->GetWorldPosition();
	CBuffer.SizeX = Transform->GetWorldScale().x;
	CBuffer.SizeY = Transform->GetWorldScale().y;

	Ezptr<AIMTransform> Tmp = Scene->GetMainCameraTransform();

	CBuffer.AxisX = Tmp->GetWorldAxis(AXIS_X);
	CBuffer.AxisY = Tmp->GetWorldAxis(AXIS_Y);

	return 0;
}

int AIMParticle::Collision(float _Time)
{
	return 0;
}

int AIMParticle::PrevRender(float _Time)
{
	ShaderManager::UpdateConstBuffer("Particle", &CBuffer);

	return 0;
}

int AIMParticle::Render(float _Time)
{
	return 0;
}

AIMParticle * AIMParticle::Clone() const
{
	return new AIMParticle(*this);
}
