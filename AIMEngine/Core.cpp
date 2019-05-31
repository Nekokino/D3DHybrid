#include "Core.h"
#include "AIMDevice.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "PathManager.h"
#include "ShaderManager.h"
#include "TimeManager.h"
#include "Timer.h"
#include "SceneManager.h"
#include "AIMShader.h"
#include "AIMMesh.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "NavigationManager.h"
#include "FontManager.h"
#include "SoundManager.h"

bool Core::Loop = true;
Core* Core::pInst = nullptr;

Core::Core()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(4955);

	memset(ClearColor, 0, sizeof(float) * 4);

	EditMode = false;
}


Core::~Core()
{
	// 여기서 스태틱으로 선언한 클래스든 싱글톤으로 선언한 클래스든 릴리즈 과정을 거쳐줘야한다.
	// 안그러면 크래쉬가 난다.
	// 아마 디바이스가 없어지면서 에러가 생기는게 아닐까 싶다.
	
	SceneManager::Release();
	SoundManager::Release();
	FontManager::Release();
	NavigationManager::Release();
	InputManager::Release();
	CollisionManager::Release();
	TimeManager::Release();
	RenderManager::Release();
	ResourceManager::Release();
	PathManager::Release();

	AIMDevice::DestroyInst();
	
}


bool Core::Init(HINSTANCE _hInst, const TCHAR * _Title, const TCHAR * _ClassName, int _Width, int _Height, int _IconID, int _SmallIconID, bool _EditMode, bool _WindowMode)
{
	hInst = _hInst;
	Rs.Width = _Width;
	Rs.Height = _Height;

	Register(_ClassName, _IconID, _SmallIconID);
	CreateWnd(_Title, _ClassName);

	return Init(hInst, hWnd, Rs.Width, Rs.Height, _EditMode, _WindowMode);
}

bool Core::Init(HINSTANCE _hInst, HWND _hWnd, int _Width, int _Height, bool _EditMode, bool _WindowMode)
{
	hInst = _hInst;
	hWnd = _hWnd;
	EditMode = _EditMode;

	if (false == GetDeviceInst->Init(hWnd, _Width, _Height, _WindowMode))
	{
		tassertmsg(true, "Device Init Failed");
		return false;
	}

	if (false == PathManager::Init())
	{
		tassertmsg(true, "PathManager Init Failed");
		return false;
	}

	if (false == FontManager::Init())
	{
		tassertmsg(true, "FontManager Init Failed");
		return false;
	}

	if (false == SoundManager::Init())
	{
		tassertmsg(true, "SoundManager Init Failed");
		return false;
	}

	if (false == InputManager::Init(hInst, hWnd))
	{
		tassertmsg(true, "InputManager Init Failed");
		return false;
	}

	if (false == ResourceManager::Init())
	{
		tassertmsg(true, "ResourceManager Init Failed");
		return false;
	}

	if (false == RenderManager::Init())
	{
		tassertmsg(true, "RenderManager Init Failed");
		return false;
	}

	if (false == TimeManager::Init())
	{
		tassertmsg(true, "TimeManager Init Failed");
		return false;
	}

	if (false == CollisionManager::Init())
	{
		tassertmsg(true, "CollisionManager Init Failed");
		return false;
	}

	if (false == NavigationManager::Init())
	{
		tassertmsg(true, "NavigationManager Init Failed");
		return false;
	}


	if (false == SceneManager::Init())
	{
		tassertmsg(true, "SceneManager Init Failed");
		return false;
	}

	return true;
}


void Core::SetClearColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	ClearColor[0] = _r / 255.0f;
	ClearColor[1] = _g / 255.0f;
	ClearColor[2] = _b / 255.0f;
	ClearColor[3] = _a / 255.0f;
}

void Core::OnEditMode()
{
	EditMode = true;
}

HINSTANCE Core::GetWindowInst() const
{
	return hInst;
}

HWND Core::GetWindowHandle() const
{
	return hWnd;
}

bool Core::GetEditMode() const
{
	return EditMode;
}

int Core::Run()
{
	MSG msg;

	while (Loop)
	{
		// 까먹을까봐
		// PeekMessage는 메세지가 없으면 false를 리턴해준다.
		// 이걸 사용해서 게임을 데드타임을 이용해서 만든다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void Core::Register(const TCHAR * _ClassName, int _IconID, int _SmallIconID)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Core::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(_IconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MY180629);
	wcex.lpszClassName = _ClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(_SmallIconID));

	RegisterClassEx(&wcex);
}

void Core::CreateWnd(const TCHAR * _Title, const TCHAR * _ClassName)
{
	hWnd = CreateWindow(_ClassName, _Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, Rs.Width, Rs.Height,
		nullptr, nullptr, hInst, nullptr);

	if (!hWnd)
		return;

	RECT		rc = { 0, 0, Rs.Width, Rs.Height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEWHEEL:
		InputManager::SetWheel(HIWORD(wParam));
		break;
	case WM_DESTROY:
		Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Core::Logic()
{
	Ezptr<Timer> Timer = TimeManager::FindTimer("MainThreadTimer");

	Timer->Update();

	float Time = Timer->GetTime();

	SoundManager::Update(Time);

	if (Input(Time) == SC_NEXT)
	{
		return;
	}

	if (Update(Time) == SC_NEXT)
	{
		return;
	}

	if (LateUpdate(Time) == SC_NEXT)
	{
		return;
	}
	Collision(Time);
	Render(Time);
}

int Core::Input(float _Time)
{
	InputManager::Update(_Time);

	return SC_NONE;
}

int Core::Update(float _Time)
{
	int sc = SceneManager::Update(_Time);

	return sc;
}

int Core::LateUpdate(float _Time)
{
	int sc = SceneManager::LateUpdate(_Time);

	return sc;
}

int Core::Collision(float _Time)
{
	CollisionManager::Collision(_Time);

	return SC_NONE;
}

int Core::Render(float _Time)
{
	GetDeviceInst->Clear(ClearColor);

	int sc = SceneManager::Render(_Time);

	RenderManager::ComputeInstancing();

	RenderManager::Render(_Time);

	InputManager::Render(_Time);
	//Ezptr<AIMShader> Shader = ShaderManager::FindShader("StandardColorShader");
	//ID3D11InputLayout* Layout = ShaderManager::FindInputLayout("StandardColorLayout");
	//Ezptr<AIMMesh> Mesh = ResourceManager::FindMesh("ColorTriangle");

	//GetAIMContext->IASetInputLayout(Layout);
	//Shader->SetShader();

	//Mesh->Render();

	GetDeviceInst->Present();

	return sc;
}
