#pragma once

#include "EngineHeader.h"

template<typename T>
class Engine_DLL AIMHeap
{
private:
	T* Array;
	int Size;
	int Capacity;
	std::function<bool(const T&, const T&)> CmpFunc;

public:
	void SetSortFunc(bool(*_Func)(const T&, const T&))
	{
		CmpFunc = std::bind(_Func, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename ClassType>
	void SetSortFunc(ClassType* _Obj, bool(ClassType::*_Func)(const T&, const T&))
	{
		CmpFunc = std::bind(_Func, _Obj, std::placeholders::_1, std::placeholders::_2);
	}

private:
	void Resize()
	{
		if (Capacity == Size)
		{
			Capacity *= 2;
			T* List = new T[Capacity];
			memset(List, 0, sizeof(T) * Size);
			memcpy(List, Array, sizeof(T) * Size);

			delete[] Array;
			Array = List;
		}
	}

public:
	void Resize(int _Capacity)
	{
		Capacity = _Capacity;

		T* List = new T[Capacity];
		memset(List, 0, sizeof(T) * Capacity);

		delete[] Array;
		Array = List;
	}

	void Insert(const T& _Data)
	{
		Resize();

		Array[Size] = _Data;

		++Size;

		_Insert(Size - 1);
	}

	bool Pop(T& _Data)
	{
		if (Size == 0)
		{
			return false;
		}

		_Data = Array[0];

		Array[0] = Array[Size - 1];
		--Size;

		_Pop(0);

		return true;
	}

	void Clear()
	{
		Size = 0;
	}

	bool Empty() const
	{
		return Size == 0;
	}

	void Sort()
	{
		int Index = Size / 2 - 1;

		while (Index >= 0)
		{
			int BuildCount = 1;
			int BuildIndex = Index + 1;

			while (BuildIndex /= 2)
			{
				BuildCount *= 2;
			}

			--BuildCount;

			BuildCount = Index - BuildCount;

			for (int i = 0; i <= BuildCount; i++)
			{
				_Pop(Index - i);
			}

			Index = Index - BuildCount - 1;
		}
	}

private:
	void _Insert(int _Index)
	{
		if (_Index == 0)
		{
			return;
		}

		int Parent = (_Index - 1) / 2;

		if (CmpFunc(Array[Parent], Array[_Index]) == true)
		{
			T Data = Array[Parent];
			Array[Parent] = Array[_Index];
			Array[_Index] = Data;

			_Insert(Parent);
		}
	}

	void _Pop(int _Index)
	{
		int LeftChild = _Index * 2 + 1;

		if (LeftChild >= Size)
		{
			return;
		}

		int RightChild = LeftChild + 1;
		int ChildIndex = LeftChild;

		if (RightChild < Size)
		{
			if (CmpFunc(Array[LeftChild], Array[RightChild]) == true)
			{
				ChildIndex = RightChild;
			}
		}

		if (CmpFunc(Array[_Index], Array[ChildIndex]) == true)
		{
			T Data = Array[ChildIndex];
			Array[ChildIndex] = Array[_Index];
			Array[_Index] = Data;

			_Pop(ChildIndex);
		}
	}

private:
	static bool Sort(const T& _Src, const T& _Dest)
	{
		return _Src > _Dest;
	}


public:
	AIMHeap()
	{
		Size = 0;
		Capacity = 100;
		Array = new T[Capacity];

		SetSortFunc(AIMHeap<T>::Sort);
	}

	~AIMHeap()
	{
		if (Array != nullptr)
		{
			delete[] Array;
			Array = nullptr;
		}
	}
};