#include "BlendState.h"
#include "AIMDevice.h"


BlendState::BlendState()
{
	Type = RS_BLEND;
	memset(BlendFactor, 0, sizeof(float) * 4);
}


BlendState::~BlendState()
{
}

bool BlendState::CreateState(const std::string & _Name, BOOL _AlphaToCoverage, BOOL _IndependentBlend)
{
	SetNameTag(_Name.c_str());

	D3D11_BLEND_DESC Desc = {};

	Desc.AlphaToCoverageEnable = _AlphaToCoverage;
	Desc.IndependentBlendEnable = _IndependentBlend;

	for (size_t i = 0; i < BlendDescVec.size(); i++)
	{
		Desc.RenderTarget[i] = BlendDescVec[i];
	}

	if (FAILED(GetAIMDevice->CreateBlendState(&Desc, (ID3D11BlendState**)&State)))
	{
		tassertmsg(true, "Failed To Create BlendState");
		return false;
	}

	return true;
}

void BlendState::AddTargetBlendDesc(BOOL _Enable, D3D11_BLEND _SrcBlend, D3D11_BLEND _DestBlend, D3D11_BLEND_OP _Op, D3D11_BLEND _SrcBlendAlpha, D3D11_BLEND _DestBlendAlpha, D3D11_BLEND_OP _AlphaOp, UINT _WriteMask)
{
	if (BlendDescVec.size() == 8)
	{
		tassertmsg(true, "Can't Create BlendState (Blend State is Over 8)");
		return;
	}

	D3D11_RENDER_TARGET_BLEND_DESC Desc = {};

	Desc.BlendEnable = _Enable;
	Desc.SrcBlend = _SrcBlend;
	Desc.DestBlend = _DestBlend;
	Desc.BlendOp = _Op;
	Desc.SrcBlendAlpha = _SrcBlendAlpha;
	Desc.DestBlendAlpha = _DestBlendAlpha;
	Desc.BlendOpAlpha = _AlphaOp;
	Desc.RenderTargetWriteMask = _WriteMask;

	BlendDescVec.push_back(Desc);
}

void BlendState::SetState()
{
	GetAIMContext->OMGetBlendState((ID3D11BlendState**)&OldState, OldBlendFactor, &OldSampleMask);

	GetAIMContext->OMSetBlendState((ID3D11BlendState*)State, BlendFactor, SampleMask);
}

void BlendState::ResetState()
{
	GetAIMContext->OMSetBlendState((ID3D11BlendState*)OldState, OldBlendFactor, OldSampleMask);

	if (OldState != nullptr)
	{
		OldState->Release();
		OldState = nullptr;
	}
}
