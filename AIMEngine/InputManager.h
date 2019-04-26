#pragma once

#include "EngineHeader.h"

typedef struct Engine_DLL _tagBindAxis
{
	std::string Name;
	std::function<void(float, float)> Func;
}BindAxis, *PBindAxis;

typedef struct Engine_DLL _tagBindAction
{
	std::string Name;
	std::function<void(float)> Func;
	int KeyState;
}BindAction, *PBindAction;

typedef struct Engine_DLL _tagKeyScale
{
	unsigned int Key;
	float Scale;
}KeyScale, *PKeyScale;

typedef struct Engine_DLL _tagKeyAxisInfo
{
	std::string Name;
	std::list<BindAxis*> BindList;
	std::list<KeyScale*> KeyList;
}KeyAxisInfo, *PKeyAxisInfo;

class Engine_DLL InputManager
{
private:
	static std::unordered_map<std::string, KeyAxisInfo*> AxisMap;

private:
	InputManager() {}
	virtual ~InputManager() = 0 {}
};

