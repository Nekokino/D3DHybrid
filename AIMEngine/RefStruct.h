#pragma once

#include "RefCounter.h"

class Engine_DLL ConstBuffer : public RefCounter
{
public:
	std::string Name;
	ID3D11Buffer* Buffer = nullptr;
	void* Data = nullptr;
	int Size = 0;
	int Register = 0;
	int Constant = 0;

private:
	~ConstBuffer()
	{
		if (Buffer != nullptr)
		{
			Buffer->Release();
			Buffer = nullptr;
		}

		if (Data != nullptr)
		{
			delete[] Data;
			Data = nullptr;
		}
	}
};

