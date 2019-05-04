#include "AIMMaterial.h"
#include "ResourceManager.h"
#include "AIMTexture.h"
#include "AIMSampler.h"
#include "ShaderManager.h"

AIMMaterial::AIMMaterial()
{
	CT = CT_MATERIAL;
}

AIMMaterial::AIMMaterial(const AIMMaterial & _Other) : AIMComponent(_Other)
{
	ContainerVec.clear();

	for (size_t AA = 0; AA < _Other.ContainerVec.size(); ++AA)
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;

		for (size_t BB = 0; BB < _Other.ContainerVec[AA]->SubsetVec.size(); ++BB)
		{
			Ezptr<MaterialSubset> Subset = new MaterialSubset;

			Subset->Mtrl = _Other.ContainerVec[AA]->SubsetVec[BB]->Mtrl;

			if (_Other.ContainerVec[AA]->SubsetVec[BB]->Diffuse != nullptr)
			{
				Subset->Diffuse = new TextureSet;
				*Subset->Diffuse = *(_Other.ContainerVec[AA]->SubsetVec[BB]->Diffuse);
			}

			Container->SubsetVec.push_back(Subset);
		}
		ContainerVec.push_back(Container);
	}
}


AIMMaterial::~AIMMaterial()
{
	ContainerVec.clear();
}

void AIMMaterial::SetColor(const Vec4 & _Dif, const Vec4 & _Amb, const Vec4 & _Spc, const Vec4 & _Emv, float _SpcPower, UINT _Container, UINT _Subset)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	memcpy(&Subset->Mtrl.Diffuse, &_Dif, sizeof(Vec4));
	memcpy(&Subset->Mtrl.Ambient, &_Amb, sizeof(Vec4));
	memcpy(&Subset->Mtrl.Emissive, &_Emv, sizeof(Vec4));
	memcpy(&Subset->Mtrl.Specular, &_Spc, sizeof(Vec4));

	Subset->Mtrl.Specular.w = _SpcPower;
}

void AIMMaterial::SetDiffuseTextureSet(int _Container, int _Subset, const std::string & _SmpName, int _SmpReg, int _TexReg, const std::string & _TexName, const TCHAR * _FileName, const std::string & _Path)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	if (Subset->Diffuse == nullptr)
	{
		Subset->Diffuse = new TextureSet;
	}

	if (Subset->Diffuse->Texture != nullptr)
	{
		Subset->Diffuse->Texture = nullptr;
	}
	if (Subset->Diffuse->Sampler != nullptr)
	{
		Subset->Diffuse->Sampler = nullptr;
	}

	if (_FileName != nullptr)
	{
		ResourceManager::LoadTexture(_TexName, _FileName, _Path);
	}

	Subset->Diffuse->Texture = ResourceManager::FindTexture(_TexName);
	Subset->Diffuse->Sampler = ResourceManager::FindSampler(_SmpName);

	Subset->Diffuse->TexReg = _TexReg;
	Subset->Diffuse->SmpReg = _SmpReg;
}

void AIMMaterial::Start()
{
}

bool AIMMaterial::Init()
{
	return true;
}

int AIMMaterial::Input(float _Time)
{
	return 0;
}

int AIMMaterial::Update(float _Time)
{
	return 0;
}

int AIMMaterial::LateUpdate(float _Time)
{
	return 0;
}

int AIMMaterial::Collision(float _Time)
{
	return 0;
}

int AIMMaterial::PrevRender(float _Time)
{
	return 0;
}

int AIMMaterial::Render(float _Time)
{
	return 0;
}

AIMMaterial * AIMMaterial::Clone() const
{
	return new AIMMaterial(*this);
}

void AIMMaterial::SetShader(int _Container, int _Subset)
{
	Ezptr<MaterialSubset> Subset = ContainerVec[_Container]->SubsetVec[_Subset];

	ShaderManager::UpdateConstBuffer("Material", &Subset->Mtrl);

	if (Subset->Diffuse != nullptr)
	{
		Subset->Diffuse->Texture->PSSetShader(Subset->Diffuse->TexReg);
		Subset->Diffuse->Sampler->PSSetSampler(Subset->Diffuse->SmpReg);
	}
}
