#pragma once

#include "EngineHeader.h"

// �˾Ƴ���
// DLL�� �Ἥ �׷����� �� �𸣰�����,
// �̱��� ���� �Ⱦ static class�� ������µ�,
// DLL�� ������ �� �� ���ڰ� �ִ� ���� �Լ��� �θ��� �Ǹ� ��ũ ������ ���. -> �� ���. �̰� ������Ʈ ���������� �ذ��ߴµ� �̰� Ȯ���غ����ҰŰ�����.
// �׷� �̱����� ����°� �´�.
// �ֳ��ϸ� �̰� ��¥ �ϳ��� �־���ϴ°Ŵϱ�

class Engine_DLL Core
{
private:
	static Core* pInst;
public:
	static Core* Inst()
	{
		if (pInst == nullptr)
		{
			pInst = new Core();
		}
		return pInst;
	}

	static void DestroyInst()
	{
		if (pInst != nullptr)
		{
			delete pInst;
			pInst = nullptr;
		}
	}

private:
	HINSTANCE hInst;
	HWND hWnd;
	Resolution Rs;
	float ClearColor[4];
	static bool Loop;
	bool EditMode;

public:
	HINSTANCE GetWindowInst() const;
	HWND GetWindowHandle() const;
	bool GetEditMode() const;

	int Run();
	bool Init(HINSTANCE _hInst, const TCHAR* _Title, const TCHAR* _ClassName, int _Width, int _Height, int _IconID, int _SmallIconID, bool _EditMode = false, bool _WindowMode = true);
	bool Init(HINSTANCE _hInst, HWND _hWnd, int _Width, int _Height, bool _EditMode =false, bool _WindowMode = true);
	bool Init(HINSTANCE _hInst, const TCHAR* _Title)
	{
		hInst = _hInst;

		return true;
	}

	void Logic();

	void SetClearColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);
	void OnEditMode();

private:
	int Input(float _Time);
	int Update(float _Time);
	int LateUpdate(float _Time);
	int Collision(float _Time);
	int Render(float _Time);


private:
	void Register(const TCHAR* _ClassName, int _IconID, int _SmallIconID);
	void CreateWnd(const TCHAR* _Title, const TCHAR* _ClassName);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// �̱��� ������ ���� �� �ʿ䰡 �ֳ�? �� �ϳ��� ��������°� �´°� �´µ�..
	// �׷��� ���� ������ ��� ����ƽ���� �� ���������⸦ ���ڴ�.
	// �̷��� �޸𸮿� ���������� ���� �������� �ʿ䵵 ����.
	// ���ʿ� �ھ ���α׷��� ���� �������� �������ϵ� ����.

	// -> ��� ó������ ���� ����.

	// (����)
	// �̱����� 
	// private:
	// static (Type*) Inst
	// public:
	// static (Type*) GetInst()
	// {
	//    if(!Inst)
	//     {
	//          Inst = new Type;
	//     }
	//     return Inst;
	// �ϰ� ����Ʈ�� ������ְ�
	// �����ڿ� �Ҹ��ڸ� private �ɾ��ָ� �ȴ�.
	// �Ƚᵵ ����� ���ڱ�

private:
	Core();
	~Core();
};

