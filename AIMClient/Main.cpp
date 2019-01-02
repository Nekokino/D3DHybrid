#include <Core.h>
#include "resource.h"

#ifdef _DEBUG
	#ifdef _M_IX86
		#pragma comment(lib, "AIMEngineD32")
	#else
		#pragma comment(lib, "AIMEngineD64")
	#endif
#else
	#ifdef _M_IX86
		#pragma comment(lib, "AIMEngineR32")
	#else
		#pragma comment(lib, "AIMEngineR64")
	#endif
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!(Core::Inst()->Init(hInstance, TEXT("Eek"), TEXT("Eek"), 1280, 720, IDI_ICON1, IDI_ICON1, true)))
	{
		Core::Inst()->DestroyInst();
		return 0;
	}

	// 왜인지는 모르겠지만.
	// 프로젝트 설정에서 유니코드로 하면 링크오류가 뜬다.
	// 멀티바이트로 하면 잘 된다.
	// 왜??????

	int	Return = Core::Inst()->Run();

	return Return;
}
