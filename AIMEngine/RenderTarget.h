#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"
#include "AIMShader.h"
#include "AIMMesh.h"

class Engine_DLL RenderTarget : public RefCounter
{
	friend class RenderManager;
private:
	std::string Name;
	float ClearColor[4] = {};
	ID3D11RenderTargetView* RTV = nullptr;
	ID3D11Texture2D* TargetTex = nullptr;
	ID3D11ShaderResourceView* SRV = nullptr;
	ID3D11DepthStencilView* DepthView = nullptr;
	ID3D11Texture2D* DepthTex = nullptr;

private:
	ID3D11RenderTargetView* OldRTV = nullptr;
	ID3D11DepthStencilView* OldDepthView = nullptr;

public:
	ID3D11RenderTargetView* GetRTV() const
	{
		return RTV;
	}

	ID3D11DepthStencilView* GetDepthView() const
	{
		return DepthView;
	}

private:
	Vec3 Pos;
	Vec3 Scale;
	bool bDebug = false;
	Matrix* Projection = nullptr;
	Ezptr<AIMMesh> Mesh;
	Ezptr<AIMShader> Shader;
	ID3D11InputLayout* Layout = nullptr;

public:
	bool CreateRenderTarget(const std::string& _Name, UINT _Width, UINT _Height, DXGI_FORMAT _Format, float _ClearColor[4], int _SampleCount =  1, DXGI_FORMAT _DepthFormat = DXGI_FORMAT_UNKNOWN);

	void OnDebug(const Vec3& _Pos, const Vec3& _Scale, bool _Debug = true);
	void ClearTarget();
	void SetTarget();
	void ResetTarget();
	void SetShader(int _Register);
	void ResetShader(int _Register);
	void Render();

private:
	RenderTarget();
	~RenderTarget();
};

