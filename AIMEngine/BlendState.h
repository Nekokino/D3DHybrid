#pragma once
#include "RenderState.h"
class Engine_DLL BlendState : public RenderState
{
	friend class RenderManager;

private:
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC> BlendDescVec;
	float BlendFactor[4] = {};
	UINT SampleMask = 0xffffffff;
	float OldBlendFactor[4] = {};
	UINT OldSampleMask = 0;

public:
	bool CreateState(const std::string& _Name, BOOL _AlphaToCoverage = FALSE, BOOL _IndependentBlend = FALSE);
	void AddTargetBlendDesc(BOOL _Enable = FALSE, D3D11_BLEND _SrcBlend = D3D11_BLEND_ONE, D3D11_BLEND _DestBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _Op = D3D11_BLEND_OP_ADD, D3D11_BLEND _SrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND _DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _AlphaOp = D3D11_BLEND_OP_ADD, UINT _WriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);

public:
	virtual void SetState() override;
	virtual void ResetState() override;
private:
	BlendState();
	~BlendState();
};

