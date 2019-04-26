#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"

class Timer;
class Engine_DLL TimeManager
{
private:
	static std::unordered_map<std::string, Ezptr<Timer>> TimerMap;

public:
	static bool Init();
	static void Release();
	static bool CreateTimer(const std::string& _Name);
	static Ezptr<Timer> FindTimer(const std::string& _Name);

private:
	TimeManager() {}
	virtual ~TimeManager() = 0 {}
};

