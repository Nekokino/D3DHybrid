#include "AIMSerialNumber.h"



void AIMSerialNumber::SetMinMax(unsigned int _Min, unsigned int _Max)
{
	Min = _Min;
	Max = _Max;
}

unsigned int AIMSerialNumber::GetSerialNumber()
{
	if (Size == 0)
	{
		unsigned int TmpNum = Number + Min;
		++Number;
		return TmpNum;
	}

	--Size;
	return ValidNumber[Size];
}

void AIMSerialNumber::AddValidNumber(unsigned int _Number)
{
	ValidNumber[Size] = _Number;
	++Size;
}

AIMSerialNumber::AIMSerialNumber()
{
	ValidNumber = new unsigned int[Capacity];
}


AIMSerialNumber::~AIMSerialNumber()
{
	if (ValidNumber != nullptr)
	{
		delete[] ValidNumber;
		ValidNumber = nullptr;
	}
}
