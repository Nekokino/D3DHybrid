#pragma once

#include "EngineHeader.h"


class Engine_DLL PathManager
{
private:
	static std::unordered_map<std::string, TCHAR*> PathMap;
	static char strChange[MAX_PATH];
	
public:
	static bool Init();
	static void Release();
	static bool CreatePath(const std::string& _PathKey, const TCHAR* _AddPath, const std::string& _BaseKey);
	static TCHAR* FindPath(const std::string& _Key);
	static const char* FindPathMultiByte(const std::string& _Key);
	

private:
	PathManager() {}
	virtual ~PathManager() = 0 {}
};

