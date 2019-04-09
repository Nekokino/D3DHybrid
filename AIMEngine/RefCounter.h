#pragma once
#include <type_traits>
#include "EngineHeader.h"

template<typename BASE, typename CHILD>
class Engine_DLL TypeCheck
{
private:
	static short Check(BASE*) {};
	static char Check(...) {};

	static CHILD* p;

public:
	static const bool Value = (bool)(sizeof(Check(p)) - 1);

private:
	TypeCheck() {}
	~TypeCheck() {}
};

class Engine_DLL RefCounter
{
public:
	template<typename T>
	friend class Ezptr;

public:
	std::string NameTag;

protected:
	unsigned int Count;

private:
	void AddRef();
	void DecRef();

public:
	void SetNameTag(const char* _Tag)
	{
		NameTag = _Tag;
	}
	std::string GetNameTag() const
	{
		return NameTag;
	}

protected:
	RefCounter();
	RefCounter(const RefCounter& _Other);
	virtual ~RefCounter() = 0; // 이거를 직접 생성할일은 없음.
};

template<typename T>
class Engine_DLL Ezptr
{


private:
	T* ptr;

public:
	Ezptr() : ptr(nullptr) {}

	Ezptr(T* _ptr) : ptr(_ptr)
	{
		AddCheck();
	}

	Ezptr(const Ezptr& _Other) : ptr(_Other.ptr)
	{
		AddCheck();
	}

	Ezptr(std::nullptr_t _ptr) : ptr(_ptr) // nullptr도 체크할수 있는 형태임미다.
	{

	}
	
	~Ezptr()
	{
		DecCheck();
	}

public:
	bool operator <(const Ezptr& _Other) const
	{
		return ptr < _Other.ptr;
	}

	bool operator >(const Ezptr& _Other) const
	{
		return ptr > _Other.ptr;
	}

	bool operator==(std::nullptr_t _ptr) const
	{
		return ptr == _ptr;
	}

	bool operator==(T* _ptr) const
	{
		return ptr == _ptr;
	}

	bool operator== (const Ezptr& _Other) const
	{
		return ptr == _Other.ptr;
	}

	bool operator!= (std::nullptr_t _ptr) const
	{
		return ptr != _ptr;
	}

	operator T*()
	{
		return ptr;
	}

	const T* operator->() const
	{
		return ptr;
	}

	T* operator->()
	{
		return ptr;
	}

	T& operator*()
	{
		return *ptr;
	}

	template<typename Type>
	operator Ezptr<Type>() // 형변환 연산자
	{
		if (nullptr == ptr)
		{
			return nullptr; 
		}

		Type* p = dynamic_cast<Type*>(ptr);

		if (true == TypeCheck<T, Type>::Value)
		{
			return (Type*)ptr;
		}

		return p;
	}

	Ezptr<T>& operator=(T* _ptr)
	{
		if (ptr == _ptr)
		{
			return *this;
		}

		DecCheck();

		ptr = _ptr;

		AddCheck();
		return *this;
	}

	Ezptr<T>& operator=(const Ezptr& _ptr)
	{
		if (ptr == _ptr.ptr)
		{
			return *this;
		}

		DecCheck();
		ptr = _ptr.ptr;
		AddCheck();

		return *this;
	}

private:
	void AddCheck()
	{
		if (nullptr != ptr)
		{
			ptr->AddRef();
		}
	}

	void DecCheck()
	{
		if (nullptr != ptr)
		{
			ptr->DecRef();
		}
	}
};

template<typename T>
bool operator==(std::nullptr_t _ptr, const Ezptr<T>& _Ezptr) { return _Ezptr == _ptr; }

template<typename T>
bool operator!=(std::nullptr_t _ptr, const Ezptr<T>& _Ezptr) { return _Ezptr != _ptr; }
