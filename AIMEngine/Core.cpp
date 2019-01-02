#include "Core.h"

bool Core::Loop = true;
Core* Core::pInst = nullptr;

Core::Core()
{
}


Core::~Core()
{
}


bool Core::Init(HINSTANCE _hInst, const TCHAR * _Title, const TCHAR * _ClassName, int _Width, int _Height, int _IconID, int _SmallIconID, bool _WindowMode)
{
	hInst = _hInst;
	Rs.Width = _Width;
	Rs.Height = _Height;

	Register(_ClassName, _IconID, _SmallIconID);
	CreateWnd(_Title, _ClassName);

	return true;
}

bool Core::Init(HINSTANCE _hInst, HWND _hWnd, int _Width, int _Height, bool _WindowMode)
{
	hInst = _hInst;
	hWnd = _hWnd;

	return true;
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
			int a = 0;
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

