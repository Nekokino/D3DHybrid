#include "Timer.h"
#include "Core.h"


float Timer::GetTime() const
{
	return DeltaTime;
}

float Timer::GetFPS() const
{
	return FPS;
}

bool Timer::Init()
{
	QueryPerformanceFrequency(&Second); // CPU 시계의 주기
	QueryPerformanceCounter(&Tick); // CPU 시계 누적 카운터

	DeltaTime = 0.0f;
	FPS = 0.0f;
	FPSTime = 0.0f;
	iTick = 0;

	return true;
}

void Timer::Update()
{
	LARGE_INTEGER Tmp;

	QueryPerformanceCounter(&Tmp); // 업데이트 함수 실행 시점까지의 누적 카운터 계산

	// 현재 카운터에서 전 프레임까지의 누적 카운터를 빼서 경과 시간 계산한걸 주기로 나눔
	// 이로인해 프레임간의 경과 시간을 측정함.
	DeltaTime = (Tmp.QuadPart - Tick.QuadPart) / (float)Second.QuadPart; 

	// 현재까지의 누적 시간으로 갱신
	Tick = Tmp;

	FPSTime += DeltaTime;
	++iTick;

	if (FPSTime >= 1.0f)
	{
		FPS = iTick / FPSTime;
		FPSTime = 0.0f;
		iTick = 0;

		char FPSstr[256] = {};

		sprintf_s(FPSstr, "FPS : %.5f", FPS);
		SetWindowTextA(Core::Inst()->GetWindowHandle(), FPSstr);
	}
}

Timer::Timer()
{
}


Timer::~Timer()
{
}
