#pragma once
#include "RenderState.h"
class Engine_DLL DepthStencilState : public RenderState
{
	friend class RenderManager;

private:
	UINT StencilRef = 0;
	UINT OldStencilRef = 0;

public:
	bool CreateState(const std::string& _Name, BOOL _Enable = TRUE, D3D11_DEPTH_WRITE_MASK _WirteMask = D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_FUNC _DepthFunc = D3D11_COMPARISON_LESS);

public:
	virtual void SetState() override;
	virtual void ResetState() override;

private:
	DepthStencilState();
	~DepthStencilState();
};

