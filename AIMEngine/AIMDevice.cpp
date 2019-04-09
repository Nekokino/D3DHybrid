#include "AIMDevice.h"

AIMDevice* AIMDevice::Inst = nullptr;

AIMDevice::AIMDevice() : Device(nullptr), Context(nullptr), SwapChain(nullptr), RenderTargetView(nullptr),
DepthStencilView(nullptr), RenderTarget2D(nullptr), Factory2D(nullptr)
{
}

AIMDevice::~AIMDevice()
{
	if (nullptr != RenderTarget2D)
	{
		RenderTarget2D->Release();
		RenderTarget2D = nullptr;
	}

	if (nullptr != Factory2D)
	{
		Factory2D->Release();
		Factory2D = nullptr;
	}

	if (nullptr != DepthStencilView)
	{
		DepthStencilView->Release();
		DepthStencilView = nullptr;
	}

	if (nullptr != RenderTargetView)
	{
		RenderTargetView->Release();
		RenderTargetView = nullptr;
	}

	if (nullptr != SwapChain)
	{
		SwapChain->Release();
		SwapChain = nullptr;
	}

	Context->ClearState();

	if (nullptr != Context)
	{
		Context->Release();
		Context = nullptr;
	}

	if (nullptr != Device)
	{
		Device->Release();
		Device = nullptr;
	}
}

ID3D11Device * AIMDevice::GetDevice()
{
	return Device;
}

ID3D11DeviceContext * AIMDevice::GetContext()
{
	return Context;
}

IDXGISwapChain * AIMDevice::GetSwapChain()
{
	return SwapChain;
}

Resolution AIMDevice::GetResolution()
{
	return RS;
}

ID2D1RenderTarget * AIMDevice::Get2DRenderTarget()
{
	return RenderTarget2D;
}

bool AIMDevice::Init(HWND _hWnd, unsigned int _Width, unsigned int _Height, bool _WindowMode)
{
	hWnd = _hWnd;
	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	RS.Width = _Width;
	RS.Height = _Height;

	Flag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL FLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL FLevel1 = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC SwapDesc = {};

	SwapDesc.BufferDesc.Width = _Width;
	SwapDesc.BufferDesc.Height = _Height;
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapDesc.BufferDesc.RefreshRate.Numerator = 1;
	SwapDesc.BufferDesc.RefreshRate.Denominator = 60;
	SwapDesc.BufferCount = 1;
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapDesc.OutputWindow = hWnd;
	SwapDesc.SampleDesc.Count = 1;
	SwapDesc.SampleDesc.Quality = 0;
	SwapDesc.Windowed = _WindowMode;
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, Flag, &FLevel, 1, D3D11_SDK_VERSION, &SwapDesc,
		&SwapChain, &Device, &FLevel1, &Context)))
	{
		return false;
	}

	ID3D11Texture2D* Buffer = nullptr;

	//COM°´Ã¼¸¦ °¡Á®¿À¸é ·¹ÆÛ·±½º Ä«¿îÅÍ°¡ Áõ°¡ÇÏ°Ô µÈ´Ù.
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&Buffer);

	Device->CreateRenderTargetView(Buffer, nullptr, &RenderTargetView);

	if (nullptr != Buffer)
	{
		Buffer->Release();
		Buffer = nullptr;
	}

	// ±íÀÌ ºä¸¦ ¸¸µì´Ï´Ù
	D3D11_TEXTURE2D_DESC DepthDesc = {};
	DepthDesc.Width = _Width;
	DepthDesc.Height = _Height;
	DepthDesc.ArraySize = 1;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.MipLevels = 1;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;

	if (FAILED(Device->CreateTexture2D(&DepthDesc, nullptr, &Buffer)))
	{
		return false;
	}

	Device->CreateDepthStencilView(Buffer, nullptr, &DepthStencilView);

	if (nullptr != Buffer)
	{
		Buffer->Release();
		Buffer = nullptr;
	}

	// ·»´õÅ¸°Ù ºä¿Í µª½º ºä¸¦ ¹­¾îÁØ´Ù.
	Context->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	D3D11_VIEWPORT Viewport = {};

	Viewport.Width = (float)_Width;
	Viewport.Height = (float)_Height;
	Viewport.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &Viewport);


	// D2D ÆÑÅä¸®
	D2D1_FACTORY_OPTIONS Option = {};
	Option.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, Option, &Factory2D)))
	{
		return false;
	}

	// 2D ·»´õ¸µ Å¸°Ù ¼³Á¤
	IDXGISurface* BackSurface = nullptr;

	SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackSurface));

	// 2D ·»´õÅ¸°Ù ¼³Á¤
	D2D1_RENDER_TARGET_PROPERTIES Prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(Factory2D->CreateDxgiSurfaceRenderTarget(BackSurface, Prop, &RenderTarget2D)))
	{
		return false;
	}

	if (nullptr != BackSurface)
	{
		BackSurface->Release();
		BackSurface = nullptr;
	}

	return true;
}

void AIMDevice::Clear(float _Color[4])
{
	Context->ClearRenderTargetView(RenderTargetView, _Color);
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void AIMDevice::Present()
{
	SwapChain->Present(0, 0);
}
