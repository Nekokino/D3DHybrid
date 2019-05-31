#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"

#define DIK_LBUTTON 0xfd
#define DIK_RBUTTON 0xfe
#define DIK_MBUTTON 0xff

class Engine_DLL KeyInfo : public RefCounter
{
public:
	bool Press = false;
	bool Push = false;
	bool Release = false;
};

class Engine_DLL KeyScale : public RefCounter
{
public:
	unsigned char Key = 0;
	float Scale = 0.0f;
};

class Engine_DLL BindAxisInfo : public RefCounter
{
public:
	std::string Name;
	bool FuncBind = false;
	std::function<void(float, float)> Func;
	std::list<Ezptr<KeyScale>> KeyList;
	bool Global = false;
};

class Engine_DLL ActionKeyInfo : public RefCounter
{
public:
	MouseButton MouseType = MB_NONE;
	unsigned char Key = 0;
	bool SKey[SKEY_END] = {};
	KeyInfo KI;
};

class Engine_DLL BindActionInfo : public RefCounter
{
public:
	std::string Name;
	bool FuncBind[KS_END] = {};
	std::function<void(float)> Func[KS_END];
	std::list<Ezptr<ActionKeyInfo>> KeyList;
	bool Global = false;
};

typedef struct Engine_DLL _tagKeyAxisInfo
{
	std::string Name;
	std::list<BindAxisInfo*> BindList;
	std::list<KeyScale*> KeyList;
}KeyAxisInfo, *PKeyAxisInfo;


class AIMObject;
class AIMColliderRay;
class AIMScene;
class Engine_DLL InputManager
{
private:
	static IDirectInput8* Input;
	static IDirectInputDevice8* Keyboard;
	static IDirectInputDevice8* MouseDevice;
	static IDirectInputDevice8* Joystick;
	static unsigned char Key[256];
	static unsigned char AddKeyList[256];
	static int AddKeySize;
	static KeyInfo KI[256];
	static short Wheel;
	static DIMOUSESTATE2 MouseState;
	static KeyInfo MouseInfo[8];
	static DIJOYSTATE2 JoystickState;
	static Vec2 WindowMousePos;
	static Vec2 WindowMouseMove;
	static Vec2 ViewportMousePos;
	static Vec2 ViewportMouseMove;
	static Ezptr<AIMObject> MouseObj;
	static Ezptr<AIMColliderRay> MouseRay;

public:
	static void SetWheel(short _Wheel);
	static short GetWheelDir();
	static bool MousePress(MouseButton _MouseButton);
	static bool MousePush(MouseButton _MouseButton);
	static bool MouseRelease(MouseButton _MouseButton);
	static Vec2 GetMouseWindowPos();
	static Vec2 GetMouseWindowMove();
	static Vec2 GetViewportMousePos();
	static Vec2 GetViewportMouseMove();
	static Ezptr<AIMColliderRay> GetMouseRay();
	static void ChangeMouseScene(Ezptr<AIMScene> _Scene);

private:
	static std::unordered_map<std::string, Ezptr<BindAxisInfo>> AxisMap;
	static std::unordered_map<std::string, Ezptr<BindActionInfo>> ActionMap;

public:
	static bool Init(HINSTANCE _hInst, HWND _hwnd);
	static void Release();
	static void Update(float _Time);
	static void Render(float _Time);
	static Ezptr<BindAxisInfo> FindAxis(const std::string& _Name);
	static Ezptr<BindActionInfo> FindAction(const std::string& _Name);

private:
	static bool ReadKeyborad();
	static bool ReadMouse();
	static bool ReadJoystick();

private:
	static void UpdateKeyboard(float _Time);
	static void UpdateMouse(float _Time);
	static void UpdateAxis(float _Time);
	static void UpdateAction(float _Time);

public:
	static void AddAxisKey(const std::string& _Name, unsigned char _Key, float _Scale);
	static void AddActionKey(const std::string& _Name, unsigned char _Key, int _SKey = 0, MouseButton _MouseButton = MB_NONE);

public:
	static void AxisGlobal(const std::string& _Name, bool _Global)
	{
		Ezptr<BindAxisInfo> Bind = FindAxis(_Name);

		if (Bind == nullptr)
		{
			return;
		}

		Bind->Global = _Global;
	}

	static void BindAxis(const std::string& _Name, void(*_Func)(float, float))
	{
		Ezptr<BindAxisInfo> Bind = FindAxis(_Name);

		if (Bind == nullptr)
		{
			Bind = new BindAxisInfo;

			Bind->Name = _Name;

			AxisMap.insert(std::unordered_map<std::string, Ezptr<BindAxisInfo>>::value_type(_Name, Bind));
		}

		Bind->Func = bind(_Func, std::placeholders::_1, std::placeholders::_2);
		Bind->FuncBind = true;
	}

	template<typename T>
	static void BindAxis(const std::string& _Name, T* _Obj, void(T::*_Func)(float, float))
	{
		Ezptr<BindAxisInfo> Bind = FindAxis(_Name);

		if (Bind == nullptr)
		{
			Bind = new BindAxisInfo;

			Bind->Name = _Name;

			AxisMap.insert(std::unordered_map<std::string, Ezptr<BindAxisInfo>>::value_type(_Name, Bind));
		}

		Bind->Func = bind(_Func, _Obj, std::placeholders::_1, std::placeholders::_2);
		Bind->FuncBind = true;
	}

	static void ActionGlobal(const std::string& _Name, bool _Global)
	{
		Ezptr<BindActionInfo> Bind = FindAction(_Name);

		if (Bind == nullptr)
		{
			return;
		}

		Bind->Global = _Global;
	}

	static void BindAction(const std::string& _Name, KeyState _KeyState, void(*_Func)(float))
	{
		Ezptr<BindActionInfo> Bind = FindAction(_Name);

		if (Bind == nullptr)
		{
			Bind = new BindActionInfo;

			Bind->Name = _Name;
			ActionMap.insert(std::unordered_map<std::string, Ezptr<BindActionInfo>>::value_type(_Name, Bind));
		}

		Bind->Func[_KeyState] = bind(_Func, std::placeholders::_1);
		Bind->FuncBind[_KeyState] = true;
	}

	template<typename T>
	static void BindAction(const std::string& _Name, KeyState _KeyState, T* _Obj, void(T::*_Func)(float))
	{
		Ezptr<BindActionInfo> Bind = FindAction(_Name);

		if (Bind == nullptr)
		{
			Bind = new BindActionInfo;

			Bind->Name = _Name;
			ActionMap.insert(std::unordered_map<std::string, Ezptr<BindActionInfo>>::value_type(_Name, Bind));
		}

		Bind->Func[_KeyState] = bind(_Func, _Obj, std::placeholders::_1);
		Bind->FuncBind[_KeyState] = true;
	}

private:
	InputManager() {}
	virtual ~InputManager() = 0 {}
};

