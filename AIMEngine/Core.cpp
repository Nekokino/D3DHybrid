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

bool Core::Loop = true;
Core* Core::pInst = nullptr;

Core::Core()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(6393);

	memset(ClearColor, 0, sizeof(float) * 4);
}


Core::~Core()
{
	// ���⼭ ����ƽ���� ������ Ŭ������ �̱������� ������ Ŭ������ ������ ������ ��������Ѵ�.
	// �ȱ׷��� ũ������ ����.
	// �Ƹ� ����̽��� �������鼭 ������ ����°� �ƴұ� �ʹ�.

	SceneManager::Release();

	CollisionManager::Release();
	InputManager::Release();
	TimeManager::Release();
	RenderManager::Release();
	ResourceManager::Release();
	PathManager::Release();

	AIMDevice::DestroyInst();
	
}


bool Core::Init(HINSTANCE _hInst, const TCHAR * _Title, const TCHAR * _ClassName, int _Width, int _Height, int _IconID, int _SmallIconID, bool _WindowMode)
{
	hInst = _hInst;
	Rs.Width = _Width;
	Rs.Height = _Height;

	Register(_ClassName, _IconID, _SmallIconID);
	CreateWnd(_Title, _ClassName);

	return Init(hInst, hWnd, Rs.Width, Rs.Height, _WindowMode);
}

bool Core::Init(HINSTANCE _hInst, HWND _hWnd, int _Width, int _Height, bool _WindowMode)
{
	hInst = _hInst;
	hWnd = _hWnd;

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

HINSTANCE Core::GetWindowInst() const
{
	return hInst;
}

HWND Core::GetWindowHandle() const
{
	return hWnd;
}

int Core::Run()
{
	MSG msg;

	while (Loop)
	{
		// ��������
		// PeekMessage�� �޼����� ������ false�� �������ش�.
		// �̰� ����ؼ� ������ ����Ÿ���� �̿��ؼ� �����.
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

	Input(Time);
	Update(Time);
	LateUpdate(Time);
	Collision(Time);
	Render(Time);
}

int Core::Input(float _Time)
{
	InputManager::Update(_Time);

	return 0;
}

int Core::Update(float _Time)
{
	SceneManager::Update(_Time);

	return 0;
}

int Core::LateUpdate(float _Time)
{
	SceneManager::LateUpdate(_Time);

	return 0;
}

int Core::Collision(float _Time)
{
	CollisionManager::Collision(_Time);

	return 0;
}

int Core::Render(float _Time)
{
	GetDeviceInst->Clear(ClearColor);

	SceneManager::Render(_Time);

	RenderManager::Render(_Time);
	//Ezptr<AIMShader> Shader = ShaderManager::FindShader("StandardColorShader");
	//ID3D11InputLayout* Layout = ShaderManager::FindInputLayout("StandardColorLayout");
	//Ezptr<AIMMesh> Mesh = ResourceManager::FindMesh("ColorTriangle");

	//GetAIMContext->IASetInputLayout(Layout);
	//Shader->SetShader();

	//Mesh->Render();

	GetDeviceInst->Present();

	return 0;
}
