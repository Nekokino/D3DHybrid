#pragma once

#include "RefCounter.h"
class Engine_DLL AIMSampler : public RefCounter
{
	friend class ResourceManager;

private:
	ID3D11SamplerState* Sampler = nullptr;

public:
	bool CreateSampler(const std::string& _Name, D3D11_FILTER _Filter, D3D11_TEXTURE_ADDRESS_MODE _AddU, D3D11_TEXTURE_ADDRESS_MODE _AddV, D3D11_TEXTURE_ADDRESS_MODE _AddW);

	void VSSetSampler(int _Register);
	void PSSetSampler(int _Register);

private:
	AIMSampler();
	~AIMSampler();
};

