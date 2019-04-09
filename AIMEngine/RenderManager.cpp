#include "RenderManager.h"
#include "ShaderManager.h"

bool RenderManager::Init()
{
	if (false == ShaderManager::Init())
	{
		tassertmsg(true, "ShaderManger Init Failed");
		return false;
	}

	return true;
}

void RenderManager::Release()
{
	ShaderManager::Release();
}
