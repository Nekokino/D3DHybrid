#include "DepthStencilState.h"
#include "AIMDevice.h"

DepthStencilState::DepthStencilState()
{
	StencilRef = 0xffffffff;
	Type = RS_DEPTH;
}


DepthStencilState::~DepthStencilState()
{
}


bool DepthStencilState::CreateState(const std::string & _Name, BOOL _Enable, D3D11_DEPTH_WRITE_MASK _WriteMask, D3D11_COMPARISON_FUNC _DepthFunc)
{
	SetNameTag(_Name.c_str());

	D3D11_DEPTH_STENCIL_DESC Desc = {};

	Desc.DepthEnable = _Enable;
	Desc.DepthWriteMask = _WriteMask;
	Desc.DepthFunc = _DepthFunc;
	Desc.StencilEnable = FALSE;
	Desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	Desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	Desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	Desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	Desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	Desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	Desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	Desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	if (FAILED(GetAIMDevice->CreateDepthStencilState(&Desc, (ID3D11DepthStencilState**)&State)))
	{
		tassertmsg(true, "Create DepthStencilState Failed");
		return false;
	}

	return true;
	
}

void DepthStencilState::SetState()
{
	GetAIMContext->OMGetDepthStencilState((ID3D11DepthStencilState**)&OldState, &OldStencilRef);

	GetAIMContext->OMSetDepthStencilState((ID3D11DepthStencilState*)State, StencilRef);
}

void DepthStencilState::ResetState()
{
	GetAIMContext->OMSetDepthStencilState((ID3D11DepthStencilState*)OldState, OldStencilRef);

	if (OldState != nullptr)
	{
		OldState->Release();
		OldState = nullptr;
	}
}

