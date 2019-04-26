#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"
class Engine_DLL Timer : public RefCounter
{
	friend class TimeManager;

private:
	LARGE_INTEGER Second;
	LARGE_INTEGER Tick;
	float DeltaTime;
	float FPS;
	float FPSTime;
	int iTick;

public:
	float GetTime() const;
	float GetFPS() const;

public:
	bool Init();
	void Update();

private:
	Timer();
	~Timer();
};

