#include "AIMParticle.h"
#include "ShaderManager.h"
#include "AIMTransform.h"
#include "AIMScene.h"
#include "AIMObject.h"
#include "AIMMaterial.h"


AIMParticle::AIMParticle()
{
	CT = CT_PARTICLE;
}

AIMParticle::AIMParticle(const AIMParticle & _Other) : AIMComponent(_Other)
{
	CBuffer = _Other.CBuffer;
	Material = nullptr;
	TextureName = _Other.TextureName;
}


AIMParticle::~AIMParticle()
{
	Material = nullptr;
}

bool AIMParticle::LoadTextureSet(const std::string & _Name, const TCHAR * _FileName, const std::string & _Path)
{
	if (Material == nullptr)
	{
		Material = Object->AddComponent<AIMMaterial>("ParticleMaterial");
	}

	if (TextureName.empty() == true)
	{
		Material->AddTextureSet(0, 0, 0, _Name, _FileName, _Path);
	}
	else
	{
		Material->ChangeTextureSet(0, 0, 0, TextureName, _Name, _FileName, _Path);
	}

	TextureName = _Name;

	return true;
}

bool AIMParticle::LoadTextureSetFromFullPath(const std::string & _Name, const TCHAR * _FullPath)
{
	if (Material == nullptr)
	{
		Material = Object->AddComponent<AIMMaterial>("ParticleMaterial");
	}

	if (TextureName.empty() == true)
	{
		Material->AddTextureSetFromFullPath(0, 0, 0, _Name, _FullPath);
	}
	else
	{
		Material->ChangeTextureSetFromFullPath(0, 0, 0, TextureName, _Name, _FullPath);
	}

	TextureName = _Name;

	return true;
}

bool AIMParticle::LoadTextureSet(const std::string & _Name, const std::vector<TCHAR*>& _FileName, const std::string & _Path)
{
	if (Material == nullptr)
	{
		Material = Object->AddComponent<AIMMaterial>("ParticleMaterial");
	}

	if (TextureName.empty() == true)
	{
		Material->AddTextureSetArray(0, 0, 10, _Name, &_FileName, _Path);
	}
	else
	{
		Material->ChangeTextureSetArray(0, 0, 10, TextureName, _Name, &_FileName, _Path);
	}

	TextureName = _Name;

	return true;
}

bool AIMParticle::LoadTextureSetFromFullPath(const std::string & _Name, const std::vector<TCHAR*>& _FullPath)
{
	if (Material == nullptr)
	{
		Material = Object->AddComponent<AIMMaterial>("ParticleMaterial");
	}

	if (TextureName.empty() == true)
	{
		Material->AddTextureSetArrayFromFullPath(0, 0, 10, _Name, &_FullPath);
	}
	else
	{
		Material->ChangeTextureSetArrayFromFullPath(0, 0, 10, TextureName, _Name, &_FullPath);
	}

	TextureName = _Name;

	return true;
}

void AIMParticle::Start()
{
	Material = Object->FindComponent<AIMMaterial>(CT_MATERIAL);
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
