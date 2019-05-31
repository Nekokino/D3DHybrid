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
	// false라면 제거과정까지
	bool Alive = true;
	// false라면 업데이트 안함.
	bool Enable = true;

	unsigned int SerialNumber = UINT_MAX;

protected:
	unsigned int Count;

private:
	void AddRef();
	void DecRef();

public:
	void SetNameTag(const std::string& _Tag)
	{
		NameTag = _Tag;
	}
	std::string GetNameTag() const
	{
		return NameTag;
	}

	bool IsAlive()
	{
		return Alive;
	}

	bool IsEnable()
	{
 		return Enable;
	}

	void SetAlive(bool _Value)
	{
		Alive = _Value;
	}

	void SetEnable(bool _Value)
	{
		Enable = _Value;
	}

	unsigned int GetSerialNumber() const
	{
		return SerialNumber;
	}

	void SetSerialNumber(unsigned int _Number)
	{
		SerialNumber = _Number;
	}
protected:
	RefCounter();
	RefCounter(const RefCounter& _Other);
	virtual ~RefCounter() = 0; // 이거를 직접 생성할일은 없음.
};


// dllexport 클래스는 모든 멤버함수와 정적 데이터 멤버가 export 된다
// 그러므로 동일한 프로그램상에서 모든 멤버가 정의되어야 한다.
// 그러니까 ezptr에 dllexport를 선언해버리면 클라이언트에서 클라이언트에서 사용할 새로운 클래스를 사용하면 링커에러가 난다.
// 그럼 얘를 exportable 클래스로 선언하면 안된다.
// 근데 dll 내에서는 dllexport던 dllimport던을 써야하지않나?
// 일단 이렇게 쓰면 에러는 안나지만 내가 편법을 쓰는 것 같다.
// https://elky.tistory.com/270 참고
template<typename T>
class Ezptr
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

	T& operator*() const
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
