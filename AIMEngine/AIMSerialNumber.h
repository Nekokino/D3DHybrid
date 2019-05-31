#pragma once

#include "EngineHeader.h"
class Engine_DLL AIMSerialNumber
{
private:
	unsigned int Number = 1;
	unsigned int* ValidNumber = nullptr;
	unsigned int Size = 0;
	unsigned int Capacity = 100;
	unsigned int Min = 0;
	unsigned int Max = 0;

public:
	void SetMinMax(unsigned int _Min, unsigned int _Max);
	unsigned int GetSerialNumber();
	void AddValidNumber(unsigned int _Number);

public:
	AIMSerialNumber();
	~AIMSerialNumber();
};

