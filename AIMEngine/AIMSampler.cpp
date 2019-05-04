#include "AIMSampler.h"
#include "AIMDevice.h"

AIMSampler::AIMSampler()
{
}


AIMSampler::~AIMSampler()
{
	if (Sampler != nullptr)
	{
		Sampler->Release();
		Sampler = nullptr;
	}
}

bool AIMSampler::CreateSampler(const std::string & _Name, D3D11_FILTER _Filter, D3D11_TEXTURE_ADDRESS_MODE _AddU, D3D11_TEXTURE_ADDRESS_MODE _AddV, D3D11_TEXTURE_ADDRESS_MODE _AddW)
{
	D3D11_SAMPLER_DESC Desc = {};

	Desc.Filter = _Filter;
	Desc.AddressU = _AddU;
	Desc.AddressV = _AddV;
	Desc.AddressW = _AddW;
	Desc.MinLOD = -FLT_MAX;
	Desc.MaxLOD = FLT_MAX;
	Desc.MipLODBias = 0.0f;
	Desc.MaxAnisotropy = 1;
	Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	memcpy(Desc.BorderColor, &Vec4(1.0f, 1.0f, 1.0f, 1.0f), sizeof(Vec4));

	if (FAILED(GetAIMDevice->CreateSamplerState(&Desc, &Sampler)))
	{
		tassertmsg(true, "Create Sampler Failed");
		return false;
	}

	SetNameTag(_Name.c_str());

	return true;

}

void AIMSampler::VSSetSampler(int _Register)
{
	GetAIMContext->VSSetSamplers(_Register, 1, &Sampler);
}

void AIMSampler::PSSetSampler(int _Register)
{
	GetAIMContext->PSSetSamplers(_Register, 1, &Sampler);
}