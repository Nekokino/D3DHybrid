#include "RasterizerState.h"
#include "AIMDevice.h"


RasterizerState::RasterizerState()
{
	Type = RS_RASTERIZER;
}


RasterizerState::~RasterizerState()
{
}

bool RasterizerState::CreateState(const std::string & _Name, D3D11_FILL_MODE _Fill, D3D11_CULL_MODE _Cull)
{
	SetNameTag(_Name.c_str());

	D3D11_RASTERIZER_DESC Desc = {};

	Desc.FillMode = _Fill;
	Desc.CullMode = _Cull;
	Desc.FrontCounterClockwise = FALSE;
	Desc.DepthBias = 0;
	Desc.SlopeScaledDepthBias = 0.0f;
	Desc.DepthBiasClamp = 0.0f;
	Desc.DepthClipEnable = TRUE;
	Desc.ScissorEnable = FALSE;
	Desc.MultisampleEnable = FALSE;
	Desc.AntialiasedLineEnable = FALSE;

	if (FAILED(GetAIMDevice->CreateRasterizerState(&Desc, (ID3D11RasterizerState**)&State)))
	{
		tassertmsg(true, "Create RasterizerState Failed");
		return false;
	}

	return true;
}

void RasterizerState::SetState()
{
	GetAIMContext->RSGetState((ID3D11RasterizerState**)&OldState);

	GetAIMContext->RSSetState((ID3D11RasterizerState*)State);
}

void RasterizerState::ResetState()
{
	GetAIMContext->RSSetState((ID3D11RasterizerState*)OldState);

	if (OldState != nullptr )
	{
		OldState->Release();
		OldState = nullptr;
	}
}