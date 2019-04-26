#include "Timer.h"



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
	QueryPerformanceFrequency(&Second); // CPU �ð��� �ֱ�
	QueryPerformanceCounter(&Tick); // CPU �ð� ���� ī����

	DeltaTime = 0.0f;
	FPS = 0.0f;
	FPSTime = 0.0f;
	iTick = 0;

	return true;
}

void Timer::Update()
{
	LARGE_INTEGER Tmp;

	QueryPerformanceCounter(&Tmp); // ������Ʈ �Լ� ���� ���������� ���� ī���� ���

	// ���� ī���Ϳ��� �� �����ӱ����� ���� ī���͸� ���� ��� �ð� ����Ѱ� �ֱ�� ����
	// �̷����� �����Ӱ��� ��� �ð��� ������.
	DeltaTime = (Tmp.QuadPart - Tick.QuadPart) / (float)Second.QuadPart; 

	// ��������� ���� �ð����� ����
	Tick = Tmp;

	FPSTime += DeltaTime;
	++iTick;

	if (FPSTime >= 1.0f)
	{
		FPS = iTick / FPSTime;
		FPSTime = 0.0f;
		iTick = 0;
	}
}

Timer::Timer()
{
}


Timer::~Timer()
{
}
