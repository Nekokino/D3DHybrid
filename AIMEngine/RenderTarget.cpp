#include "RenderTarget.h"
#include "AIMDevice.h"
#include "AIMMesh.h"
#include "ResourceManager.h"
#include "AIMShader.h"
#include "ShaderManager.h"

RenderTarget::RenderTarget()
{
	Projection = new Matrix;
}


RenderTarget::~RenderTarget()
{
	if (Projection != nullptr)
	{
		delete Projection;
		Projection = nullptr;
	}

	//if (Layout != nullptr)
	//{
	//	//여기서 지울 필요가 있나?
	//	Layout->Release();
	//	Layout = nullptr;
	//}

	if (RTV != nullptr)
	{
		RTV->Release();
		RTV = nullptr;
	}

	if (TargetTex != nullptr)
	{
		TargetTex->Release();
		TargetTex = nullptr;
	}

	if (SRV != nullptr)
	{
		SRV->Release();
		SRV = nullptr;
	}

	if (DepthView != nullptr)
	{
		DepthView->Release();
		DepthView = nullptr;
	}

	if (DepthTex != nullptr)
	{
		DepthTex->Release();
		DepthTex = nullptr;
	}

	if (OldRTV != nullptr)
	{
		OldRTV->Release();
		OldRTV = nullptr;
	}

	if (OldDepthView != nullptr)
	{
		OldDepthView->Release();
		OldDepthView = nullptr;
	}
}

bool RenderTarget::CreateRenderTarget(const std::string & _Name, UINT _Width, UINT _Height, DXGI_FORMAT _Format, float _ClearColor[4], int _SampleCount, DXGI_FORMAT _DepthFormat)
{
	Name = _Name;

	memcpy(ClearColor, _ClearColor, sizeof(float) * 4);

	D3D11_TEXTURE2D_DESC Desc = {};

	Desc.Width = _Width;
	Desc.Height = _Height;
	Desc.ArraySize = 1;
	Desc.Format = _Format;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.SampleDesc.Count = 1;
	Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(GetAIMDevice->CreateTexture2D(&Desc, nullptr, &TargetTex)))
	{
		tassertmsg(true, "Failed to Create Texture2D");
		return false;
	}

	if (FAILED(GetAIMDevice->CreateShaderResourceView(TargetTex, nullptr, &SRV)))
	{
		tassertmsg(true, "Failed to Create ShaderResourceView");
		return false;
	}

	if (FAILED(GetAIMDevice->CreateRenderTargetView(TargetTex, nullptr, &RTV)))
	{
		tassertmsg(true, "Failed to Create RenderTargetView");
		return false;
	}

	if (_DepthFormat != DXGI_FORMAT_UNKNOWN)
	{
		Desc.Format = _DepthFormat;
		Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		if (FAILED(GetAIMDevice->CreateTexture2D(&Desc, nullptr, &DepthTex)))
		{
			tassertmsg(true, "Failed to Create Texture2D");
			return false;
		}

		if (FAILED(GetAIMDevice->CreateDepthStencilView(DepthTex, nullptr, &DepthView)))
		{
			tassertmsg(true, "Failed to Create DepthStencilView");
			return false;
		}
	}

	*Projection = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, GetDeviceInst->GetResolution().Width, 0.0f, GetDeviceInst->GetResolution().Height, 0.0f, 1000.0f);

	return true;
}

void RenderTarget::OnDebug(const Vec3& _Pos, const Vec3& _Scale, bool _Debug)
{
	bDebug = _Debug;
	Pos = _Pos;
	Scale = _Scale;

	if (Mesh == nullptr)
	{
		Mesh = ResourceManager::FindMesh("Rect");
		Shader = ShaderManager::FindShader("DebugShader");
		Layout = ShaderManager::FindInputLayout("StandardUVLayout");
	}
}

void RenderTarget::ClearTarget()
{
	GetAIMContext->ClearRenderTargetView(RTV, ClearColor);

	if (DepthView != nullptr)
	{
		GetAIMContext->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void RenderTarget::SetTarget()
{
	GetAIMContext->OMGetRenderTargets(1, &OldRTV, &OldDepthView);

	ID3D11DepthStencilView* Depth = DepthView;

	if (Depth != nullptr)
	{
		Depth = OldDepthView;
	}

	GetAIMContext->OMSetRenderTargets(1, &RTV, Depth);
}

void RenderTarget::ResetTarget()
{
	GetAIMContext->OMSetRenderTargets(1, &OldRTV, OldDepthView);

	if (OldRTV != nullptr)
	{
		OldRTV->Release();
		OldRTV = nullptr;
	}

	if (OldDepthView != nullptr)
	{
		OldDepthView->Release();
		OldDepthView = nullptr;
	}
}

void RenderTarget::SetShader(int _Register)
{
	GetAIMContext->PSSetShaderResources(_Register, 1, &SRV);
}

void RenderTarget::ResetShader(int _Register)
{
	ID3D11ShaderResourceView* tSRV = nullptr;
	GetAIMContext->PSSetShaderResources(_Register, 1, &tSRV);
}

void RenderTarget::Render()
{
	if (bDebug == false)
	{
		return;
	}

	Shader->SetShader();

	Matrix ScaleMat, TranslateMat;
	ScaleMat.Scaling(Scale);
	TranslateMat.Translation(Pos);

	DebugConstBuffer CBuffer = {};

	CBuffer.WVP = ScaleMat * TranslateMat * (*Projection);
	CBuffer.WVP.Transpose();

	ShaderManager::UpdateConstBuffer("Debug", &CBuffer);

	SetShader(0);

	GetAIMContext->IASetInputLayout(Layout);

	Mesh->Render();

	ResetShader(0);
}
