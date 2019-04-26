#include "TimeManager.h"
#include "Timer.h"

std::unordered_map<std::string, Ezptr<Timer>> TimeManager::TimerMap;


bool TimeManager::Init()
{
	CreateTimer("MainThreadTimer");

	return true;
}

void TimeManager::Release()
{
	TimerMap.clear();
}

bool TimeManager::CreateTimer(const std::string & _Name)
{
	Ezptr<Timer> NewTimer = FindTimer(_Name);

	if (NewTimer != nullptr)
	{
		tassertmsg(true, "Timer Create Error");
		return false;
	}

	NewTimer = new Timer;

	if (NewTimer->Init() == false)
	{
		return false;
	}

	TimerMap.insert(std::unordered_map<std::string, Ezptr<Timer>>::value_type(_Name, NewTimer));

	return true;
}

Ezptr<Timer> TimeManager::FindTimer(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<Timer>>::iterator FindIter = TimerMap.find(_Name);

	if (FindIter == TimerMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}
