#pragma once

#include "EngineHeader.h"

#define GetDeviceInst AIMDevice::GetInst()
#define GetAIMDevice AIMDevice::GetInst()->GetDevice()
#define GetAIMContext AIMDevice::GetInst()->GetContext()

class Engine_DLL AIMDevice
{
//ΩÃ±€≈Ê
private:
	static AIMDevice* Inst;

public:
	static AIMDevice* GetInst()
	{
		if (nullptr == Inst)
		{
			Inst = new AIMDevice;
		}
		return Inst;
	}

	static void DestroyInst()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:
	AIMDevice();
	~AIMDevice();

private:
	HWND hWnd;
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;
	Resolution RS;

private:
	ID2D1RenderTarget* RenderTarget2D;
	ID2D1Factory* Factory2D;

public:
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	IDXGISwapChain* GetSwapChain();
	Resolution GetResolution();
	ID2D1RenderTarget* Get2DRenderTarget();
	Vec2 GetWindowToViewportRatio() const;

public:
	bool Init(HWND _hWnd, unsigned int _Width, unsigned int _Height, bool _WindowMode = true);
	void Clear(float _Color[4]);
	void Present();

};

