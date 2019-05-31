#include "RefCounter.h"

RefCounter::RefCounter() : Count(0)
{
}

RefCounter::RefCounter(const RefCounter & _Other) : Count(0), NameTag(_Other.NameTag), Alive(_Other.Alive), Enable(_Other.Enable), SerialNumber(_Other.SerialNumber)
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

