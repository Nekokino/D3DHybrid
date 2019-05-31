#pragma once

#include "EngineHeader.h"

// 알아낸거
// DLL을 써서 그런지는 잘 모르겠지만,
// 싱글톤 쓰기 싫어서 static class로 만들었는데,
// DLL을 가져다 쓸 때 인자가 있는 전역 함수를 부르게 되면 링크 에러가 뜬다. -> 어 잠깐. 이거 프로젝트 오류때문에 해결했는데 이거 확인해봐야할거같은데.
// 그럼 싱글톤을 만드는게 맞다.
// 왜냐하면 이건 진짜 하나만 있어야하는거니까

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

	// 싱글톤 선언을 굳이 할 필요가 있나? 뭐 하나만 만들어지는게 맞는건 맞는데..
	// 그래서 저는 예전에 배운 스태틱으로 다 조져버리기를 쓰겠다.
	// 이러면 메모리에 상주하지만 내가 해제해줄 필요도 없다.
	// 애초에 코어가 프로그램을 끄기 전까지는 해제할일도 없다.

	// -> 헤더 처음으로 가서 읽자.

	// (참고)
	// 싱글톤은 
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
	// 하고 딜리트도 만들어주고
	// 생성자와 소멸자를 private 걸어주면 된다.
	// 안써도 까먹진 말자구

private:
	Core();
	~Core();
};

