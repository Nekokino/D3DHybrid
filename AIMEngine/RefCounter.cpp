#include "RefCounter.h"

RefCounter::RefCounter() : Count(0)
{
}

RefCounter::RefCounter(const RefCounter & _Other) : Count(0)
{
}


RefCounter::~RefCounter()
{
}


void RefCounter::AddRef()
{
	++Count;
}

void RefCounter::DecRef()
{
	--Count;

	if (0 >= Count)
	{
		delete this;
	}
}

