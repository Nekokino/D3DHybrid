#pragma once

#include "RenderState.h"
class Engine_DLL RasterizerState : public RenderState
{
	friend class RenderManager;

public:
	bool CreateState(const std::string& _Name, D3D11_FILL_MODE _Fill = D3D11_FILL_SOLID, D3D11_CULL_MODE _Cull = D3D11_CULL_BACK);

public:
	virtual void SetState() override;
	virtual void ResetState() override;

private:
	RasterizerState();
	~RasterizerState();
};

