#include <Core.h>
#include "resource.h"

#include <AIMScene.h>
#include <SceneManager.h>

#include "MainScene.h"

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
	new int;

	if (false == (Core::Inst()->Init(hInstance, TEXT("Hello"), TEXT("Eek"), 1280, 720, IDI_ICON1, IDI_ICON1, true)))
	{
		Core::Inst()->DestroyInst();
		return 0;
	}

	SceneManager::AddSceneComponent<MainScene>();

	int	Return = Core::Inst()->Run();

	Core::DestroyInst();

	return Return;
}
