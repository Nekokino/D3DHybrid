#include "InputManager.h"
#include "Core.h"
#include "AIMDevice.h"
#include "AIMObject.h"
#include "AIMRenderer.h"
#include "AIMMaterial.h"
#include "AIMColliderRay.h"
#include "AIMColliderPoint.h"

std::unordered_map<std::string, Ezptr<BindAxisInfo>> InputManager::AxisMap;
std::unordered_map<std::string, Ezptr<BindActionInfo>> InputManager::ActionMap;

IDirectInput8* InputManager::Input = nullptr;
IDirectInputDevice8* InputManager::Keyboard = nullptr;
IDirectInputDevice8* InputManager::MouseDevice = nullptr;
IDirectInputDevice8* InputManager::Joystick = nullptr;
unsigned char InputManager::Key[256] = {};
unsigned char InputManager::AddKeyList[256] = {};
int InputManager::AddKeySize = 0;
KeyInfo InputManager::KI[256] = {};
short InputManager::Wheel = 0;
DIMOUSESTATE2 InputManager::MouseState;
KeyInfo InputManager::MouseInfo[8] = {};
DIJOYSTATE2 InputManager::JoystickState;

Vec2 InputManager::WindowMousePos;
Vec2 InputManager::WindowMouseMove;
Vec2 InputManager::ViewportMousePos;
Vec2 InputManager::ViewportMouseMove;
Ezptr<AIMObject> InputManager::MouseObj;
Ezptr<AIMColliderRay> InputManager::MouseRay;


void InputManager::SetWheel(short _Wheel)
{
	Wheel = _Wheel / 120;
}

short InputManager::GetWheelDir()
{
	return Wheel;
}

bool InputManager::MousePress(MouseButton _MouseButton)
{
	return MouseInfo[_MouseButton].Press;
}

bool InputManager::MousePush(MouseButton _MouseButton)
{
	return MouseInfo[_MouseButton].Push;
}

bool InputManager::MouseRelease(MouseButton _MouseButton)
{
	return MouseInfo[_MouseButton].Release;
}

Vec2 InputManager::GetMouseWindowPos()
{
	return WindowMousePos;
}

Vec2 InputManager::GetMouseWindowMove()
{
	return WindowMouseMove;
}

Vec2 InputManager::GetViewportMousePos()
{
	return ViewportMousePos;
}

Vec2 InputManager::GetViewportMouseMove()
{
	return ViewportMouseMove;
}

Ezptr<AIMColliderRay> InputManager::GetMouseRay()
{
	return MouseRay;
}

void InputManager::ChangeMouseScene(Ezptr<AIMScene> _Scene)
{
	MouseObj->SetScene(_Scene);
}

bool InputManager::Init(HINSTANCE _hInst, HWND _hwnd)
{
	if (FAILED(DirectInput8Create(_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&Input, nullptr)))
	{
		tassertmsg(true, "DirectInput Init Faild");
		return false;
	}

	if (FAILED(Input->CreateDevice(GUID_SysKeyboard, &Keyboard, nullptr)))
	{
		tassertmsg(true, "Keyboard Device Create Failed");
		return false;
	}

	if (FAILED(Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		tassert(true);
		return false;
	}

	if (FAILED(Input->CreateDevice(GUID_SysMouse, &MouseDevice, nullptr)))
	{
		tassert(true);
		return false;
	}

	if (FAILED(MouseDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		tassert(true);
		return false;
	}

	//if (FAILED(Input->CreateDevice(GUID_Joystick, &Joystick, nullptr)))
	//{
	//	//tassert(true);
	//	//return false;
	//}

	//if (FAILED(Joystick->SetDataFormat(&c_dfDIJoystick)))
	//{
	//	//tassert(true);
	//	//return false;
	//}

	AddActionKey("MouseRButton", DIK_RBUTTON, 0, MB_RBUTTON);

	MouseObj = AIMObject::CreateObject("Mouse");

	MouseRay = MouseObj->AddComponent<AIMColliderRay>("MouseRay");

	MouseRay->PickEnable();
	MouseRay->MouseEnable();
	MouseRay->EnableCollisionManager(false);

	return true;
}

void InputManager::Release()
{
	MouseObj = nullptr;
	MouseRay = nullptr;

	if (MouseDevice != nullptr)
	{
		MouseDevice->Unacquire();
		MouseDevice->Release();
		MouseDevice = nullptr;
	}

	if (Joystick != nullptr)
	{
		Joystick->Unacquire();
		Joystick->Release();
		Joystick = nullptr;
	}

	if (Keyboard != nullptr)
	{
		Keyboard->Unacquire();
		Keyboard->Release();
		Keyboard = nullptr;
	}

	if (nullptr != Input)
	{
		Input->Release();
		Input = nullptr;
	}
}

void InputManager::Update(float _Time)
{
	ReadKeyborad();
	ReadMouse();
	ReadJoystick();

	UpdateKeyboard(_Time);
	UpdateMouse(_Time);
	UpdateAxis(_Time);
	UpdateAction(_Time);

	MouseObj->Update(_Time);
	MouseObj->LateUpdate(_Time);
}

void InputManager::Render(float _Time)
{
	Wheel = 0;

	MouseObj->Render(_Time);
}

Ezptr<BindAxisInfo> InputManager::FindAxis(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<BindAxisInfo>>::iterator FindIter = AxisMap.find(_Name);

	if (FindIter == AxisMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

Ezptr<BindActionInfo> InputManager::FindAction(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<BindActionInfo>>::iterator FindIter = ActionMap.find(_Name);

	if (FindIter == ActionMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

bool InputManager::ReadKeyborad()
{
	HRESULT Result = Keyboard->GetDeviceState(256, Key);

	if (FAILED(Result))
	{
		if (Result == DIERR_INPUTLOST || Result == DIERR_NOTACQUIRED)
		{
			if (FAILED(Keyboard->SetCooperativeLevel(Core::Inst()->GetWindowHandle(), DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
			{
				return false;
			}
			Keyboard->Acquire();
		}
		else
			return false;
	}

	return true;
}

bool InputManager::ReadMouse()
{
	HRESULT Result = MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState);

	if (FAILED(Result))
	{
		if (Result == DIERR_INPUTLOST || Result == DIERR_NOTACQUIRED)
		{
			if (FAILED(MouseDevice->SetCooperativeLevel(Core::Inst()->GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
			{
				return false;
			}
			MouseDevice->Acquire();
		}
		else
			return false;
	}

	return true;
}

bool InputManager::ReadJoystick()
{
	return false;
}

void InputManager::UpdateKeyboard(float _Time)
{
	for (int i = 0; i < AddKeySize; i++)
	{
		unsigned char TmpKey = AddKeyList[i];

		if (Key[TmpKey] & 0x80)
		{
			if (KI[TmpKey].Press == false && KI[TmpKey].Push == false)
			{
				KI[TmpKey].Press = true;
				KI[TmpKey].Push = true;
			}
			else
			{
				KI[TmpKey].Press = false;
			}
		}
		else if (KI[TmpKey].Press == true || KI[TmpKey].Push == true)
		{
			KI[TmpKey].Press = false;
			KI[TmpKey].Push = false;
			KI[TmpKey].Release = true;
		}
		else if (KI[TmpKey].Release == true)
		{
			KI[TmpKey].Release = false;
		}
	}
}

void InputManager::UpdateMouse(float _Time)
{
	for (int i = 0; i < 8; i++)
	{
		if (MouseState.rgbButtons[i] & 0x80)
		{
			if (MouseInfo[i].Press == false && MouseInfo[i].Push == false)
			{
				MouseInfo[i].Press = true;
				MouseInfo[i].Push = true;
			}
			else
			{
				MouseInfo[i].Press = false;
			}
		}
		else if (MouseInfo[i].Press == true || MouseInfo[i].Push == true)
		{
			MouseInfo[i].Press = false;
			MouseInfo[i].Push = false;
			MouseInfo[i].Release = true;
		}
		else if (MouseInfo[i].Release == true)
		{
			MouseInfo[i].Release = false;
		}
	}

	POINT TmpMouse;
	GetCursorPos(&TmpMouse);
	ScreenToClient(Core::Inst()->GetWindowHandle(), &TmpMouse);

	Vec2 MousePos;
	MousePos.x = TmpMouse.x;
	MousePos.y = TmpMouse.y;

	WindowMouseMove = MousePos - WindowMousePos;
	WindowMousePos = MousePos;

	Vec2 Ratio = GetDeviceInst->GetWindowToViewportRatio();

	MousePos *= Ratio;
	ViewportMouseMove = MousePos - ViewportMousePos;
	ViewportMousePos = MousePos;
}

void InputManager::UpdateAxis(float _Time)
{
	std::unordered_map<std::string, Ezptr<BindAxisInfo>>::iterator AxisStartIter = AxisMap.begin();
	std::unordered_map<std::string, Ezptr<BindAxisInfo>>::iterator AxisEndIter = AxisMap.end();

	for (; AxisStartIter != AxisEndIter ; ++AxisStartIter)
	{

		std::list<Ezptr<KeyScale>>::iterator ScaleStartIter = AxisStartIter->second->KeyList.begin();
		std::list<Ezptr<KeyScale>>::iterator ScaleEndIter = AxisStartIter->second->KeyList.end();

		for (; ScaleStartIter != ScaleEndIter; ++ScaleStartIter)
		{
			float Scale = 0.0f;

			if (Key[(*ScaleStartIter)->Key] & 0x80)
			{
				Scale = (*ScaleStartIter)->Scale;
			}

			if (AxisStartIter->second->FuncBind == true)
			{
				AxisStartIter->second->Func(Scale, _Time);
			}
		}
	}
}

void InputManager::UpdateAction(float _Time)
{
	std::unordered_map<std::string, Ezptr<BindActionInfo>>::iterator StartIter = ActionMap.begin();
	std::unordered_map<std::string, Ezptr<BindActionInfo>>::iterator EndIter = ActionMap.end();

	const unsigned char SKey[SKEY_END] = { DIK_LCONTROL, DIK_LALT, DIK_LSHIFT };
	bool KS[SKEY_END] = {};

	for (int i = 0; i < SKEY_END; i++)
	{
		if (Key[SKey[i]] & 0x80)
		{
			KS[i] = true;
		}
		else
			KS[i] = false;
	}

	for (; StartIter != EndIter; ++StartIter)
	{
		std::list<Ezptr<ActionKeyInfo>>::iterator StartIter2 = StartIter->second->KeyList.begin();
		std::list<Ezptr<ActionKeyInfo>>::iterator EndIter2 = StartIter->second->KeyList.end();

		bool FuncCall[KS_END] = {};

		for (; StartIter2 != EndIter2; ++StartIter2)
		{
			bool SKeyEnable = true;

			for (int i = 0; i < SKEY_END; i++)
			{
				if ((*StartIter2)->SKey[i] == true)
				{
					if (KS[i] == false)
					{
						SKeyEnable = false;
						break;
					}
				}
				else
				{
					if (KS[i] == true)
					{
						SKeyEnable = false;
						break;
					}
				}
			}

			if ((*StartIter2)->MouseType == MB_NONE)
			{
				if (KI[(*StartIter2)->Key].Press == true && SKeyEnable == true)
				{
					(*StartIter2)->KI.Press = true;
					(*StartIter2)->KI.Push = true;

					FuncCall[KS_PRESS] = true;
				}
				else if (KI[(*StartIter2)->Key].Push == true && SKeyEnable == true)
				{
					(*StartIter2)->KI.Press = false;
					(*StartIter2)->KI.Push = true;

					FuncCall[KS_PUSH] = true;
				}
				else if ((*StartIter2)->KI.Push == true && (false == SKeyEnable || KI[(*StartIter2)->Key].Release == true))
				{
					(*StartIter2)->KI.Push = false;
					(*StartIter2)->KI.Release = true;

					FuncCall[KS_RELEASE] = true;
				}
				else if ((*StartIter2)->KI.Release == true)
				{
					(*StartIter2)->KI.Release = false;
				}
			}
			else
			{

			}
		}

		if (FuncCall[KS_PRESS] == true && StartIter->second->FuncBind[KS_PRESS] == true)
		{
			StartIter->second->Func[KS_PRESS](_Time);
		}

		if (FuncCall[KS_PUSH] == true && StartIter->second->FuncBind[KS_PUSH] == true)
		{
			StartIter->second->Func[KS_PUSH](_Time);
		}

		if (FuncCall[KS_RELEASE] == true && StartIter->second->FuncBind[KS_RELEASE] == true)
		{
			StartIter->second->Func[KS_RELEASE](_Time);
		}
	}
}

void InputManager::AddAxisKey(const std::string & _Name, unsigned char _Key, float _Scale)
{
	Ezptr<BindAxisInfo> Bind = FindAxis(_Name);

	if (Bind == nullptr)
	{
		Bind = new BindAxisInfo;
		Bind->Name = _Name;
		AxisMap.insert(std::unordered_map<std::string, Ezptr<BindAxisInfo>>::value_type(_Name, Bind));
	}

	std::list<Ezptr<KeyScale>>::iterator StartIter = Bind->KeyList.begin();
	std::list<Ezptr<KeyScale>>::iterator EndIter = Bind->KeyList.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		if ((*StartIter)->Key == _Key)
		{
			return;
		}
	}

	Ezptr<KeyScale> KS = new KeyScale;

	KS->Scale = _Scale;
	KS->Key = _Key;

	bool Find = false;

	for (int i = 0; i < AddKeySize; i++)
	{
		if (AddKeyList[i] == _Key)
		{
			Find = true;
			break;
		}
	}

	if (Find == false)
	{
		AddKeyList[AddKeySize] = _Key;
		++AddKeySize;
	}

	Bind->KeyList.push_back(KS);
}

void InputManager::AddActionKey(const std::string & _Name, unsigned char _Key, int _SKey, MouseButton _MouseButton)
{
	Ezptr<BindActionInfo> Bind = FindAction(_Name);

	if (Bind == nullptr)
	{
		Bind = new BindActionInfo;
		Bind->Name = _Name;
		ActionMap.insert(std::unordered_map<std::string, Ezptr<BindActionInfo>>::value_type(_Name, Bind));
	}

	std::list<Ezptr<ActionKeyInfo>>::iterator StartIter = Bind->KeyList.begin();
	std::list<Ezptr<ActionKeyInfo>>::iterator EndIter = Bind->KeyList.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		if ((*StartIter)->Key == _Key)
		{
			return;
		}
	}

	Ezptr<ActionKeyInfo> KA = new ActionKeyInfo;

	KA->Key = _Key;
	KA->MouseType = _MouseButton;

	if (_SKey & SE_CTRL)
	{
		KA->SKey[SKEY_CTRL] = true;
	}

	if (_SKey & SE_ALT)
	{
		KA->SKey[SKEY_ALT] = true;
	}

	if (_SKey & SE_SHIFT)
	{
		KA->SKey[SKEY_SHIFT] = true;
	}

	bool Find = false;

	for (int i = 0; i < AddKeySize; i++)
	{
		if (AddKeyList[i] == _Key)
		{
			Find = true;
			break;
		}
	}

	if (Find == false)
	{
		AddKeyList[AddKeySize] = _Key;
		++AddKeySize;
	}

	Bind->KeyList.push_back(KA);
}
