#pragma once

#include "EngineHeader.h"
class RenderManager
{
public:
	static bool Init();
	static void Release();
private:
	RenderManager() {}
	virtual ~RenderManager() = 0 {}
};

