#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"


class Engine_DLL KeyScale : public RefCounter
{
public:
	unsigned char Key = 0;
	float Scale = 0.0f;
};

class Engine_DLL BindAxis : public RefCounter
{
public:
	std::string Name;
	bool FuncBind = false;
	std::function<void(float, float)> Func;
	std::list<Ezptr<KeyScale>> KeyList;
};

class Engine_DLL ActionKeyInfo : public RefCounter
{
public:
	unsigned char Key = 0;
	bool SKey[SKEY_END] = {};
	bool KeyState[SKEY_END] = {};
	bool SKeyState[SKEY_END][KS_END] = {};
};

class Engine_DLL BindAction : public RefCounter
{
public:
	std::string Name;
	bool FuncBind = false;
	std::function<void(float)> Func;
	std::list<Ezptr<ActionKeyInfo>> KeyList;
	KeyState KS = KS_END;
};

typedef struct Engine_DLL _tagKeyAxisInfo
{
	std::string Name;
	std::list<BindAxis*> BindList;
	std::list<KeyScale*> KeyList;
}KeyAxisInfo, *PKeyAxisInfo;

class Engine_DLL InputManager
{
private:
	static IDirectInput8* Input;
	static IDirectInputDevice8* Keyboard;
	static IDirectInputDevice8* Mouse;
	static unsigned char Key[256];
	static bool bPress[256];
	static bool bPush[256];
	static bool bRelease[256];

private:
	static std::unordered_map<std::string, Ezptr<BindAxis>> AxisMap;
	static std::unordered_map<std::string, Ezptr<BindAction>> ActionMap;

public:
	static bool Init(HINSTANCE _hInst, HWND _hwnd);
	static void Release();
	static void Update(float _Time);
	static Ezptr<BindAxis> FindAxis(const std::string& _Name);
	static Ezptr<BindAction> FindAction(const std::string& _Name);

private:
	static bool ReadKeyborad();

public:
	static Ezptr<BindAxis> InputBindAxis(const std::string& _Name, unsigned char _Key, float _Scale, void(*_Func)(float, float))
	{
		Ezptr<BindAxis> Bind = FindAxis(_Name);

		if (Bind == nullptr)
		{
			Bind = new BindAxis;

			AxisMap.insert(std::unordered_map<std::string, Ezptr<BindAxis>>::value_type(_Name, Bind));
		}

		Bind->Name = _Name;
		Bind->Func = std::bind(_Func, std::placeholders::_1, std::placeholders::_2);
		Bind->FuncBind = true;

		Ezptr<KeyScale> KS = new KeyScale;

		KS->Key = _Key;
		KS->Scale = _Scale;

		Bind->KeyList.push_back(KS);

		return Bind;
	}

	template <typename T>
	static Ezptr<BindAxis> InputBindAxis(const std::string& _Name, unsigned char _Key, float _Scale, T* _Obj, void(T::*_Func)(float, float))
	{
		Ezptr<BindAxis> Bind = FindAxis(_Name);

		if (Bind == nullptr)
		{
			Bind = new BindAxis;
			AxisMap.insert(std::unordered_map<std::string, Ezptr<BindAxis>>::value_type(_Name, Bind));
		}

		Bind->Name = _Name;
		Bind->Func = std::bind(_Func, _Obj, std::placeholders::_1, std::placeholders::_2);
		Bind->FuncBind = true;

		Ezptr<KeyScale> KS = new KeyScale;

		KS->Key = _Key;
		KS->Scale = _Scale;

		Bind->KeyList.push_back(KS);

		return Bind;
	}

	static Ezptr<BindAction> InputBindAction(const std::string& _Name, unsigned char _Key, KeyState _KeyState, void(*_Func)(float), bool* _SKey = nullptr)
	{
		Ezptr<BindAction> Bind = FindAction(_Name);

		if (Bind == nullptr)
		{
			Bind = new BindAction;

			ActionMap.insert(std::unordered_map<std::string, Ezptr<BindAction>>::value_type(_Name, Bind));
		}

		Bind->Name = _Name;
		Bind->Func = std::bind(_Func, std::placeholders::_1);
		Bind->FuncBind = true;
		Bind->KS = _KeyState;

		Ezptr<ActionKeyInfo> KeyInfo = new ActionKeyInfo;

		KeyInfo->Key = _Key;

		if (nullptr != _SKey)
		{
			memcpy(KeyInfo->SKey, _SKey, sizeof(bool) * SKEY_END);
		}

		Bind->KeyList.push_back(KeyInfo);

		return Bind;
	}

	template<typename T>
	static Ezptr<BindAction> InputBindAction(const std::string& _Name, unsigned char _Key, KeyState _KeyState, T* _Obj, void(T::*_Func)(float), bool* _SKey = nullptr)
	{
		Ezptr<BindAction> Bind = FindAction(_Name);

		if (Bind == nullptr)
		{
			Bind = new BindAction;

			ActionMap.insert(std::unordered_map<std::string, Ezptr<BindAction>>::value_type(_Name, Bind));
		}

		Bind->Name = _Name;
		Bind->Func = std::bind(_Func, _Obj, std::placeholders::_1);
		Bind->FuncBind = true;
		Bind->KS = _KeyState;

		Ezptr<ActionKeyInfo> KeyInfo = new ActionKeyInfo;

		KeyInfo->Key = _Key;

		if (_SKey != nullptr)
		{
			memcpy(KeyInfo->SKey, _SKey, sizeof(bool) * SKEY_END);
		}

		Bind->KeyList.push_back(KeyInfo);

		return Bind;
	}

private:
	InputManager() {}
	virtual ~InputManager() = 0 {}
};

