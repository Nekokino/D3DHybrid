#include "InputManager.h"

IDirectInput8* InputManager::Input = nullptr;
IDirectInputDevice8* InputManager::Keyboard = nullptr;
IDirectInputDevice8* InputManager::Mouse = nullptr;
unsigned char InputManager::Key[256] = {};
std::unordered_map<std::string, Ezptr<BindAxis>> InputManager::AxisMap;
std::unordered_map<std::string, Ezptr<BindAction>> InputManager::ActionMap;
bool InputManager::bPress[256] = {};
bool InputManager::bPush[256] = {};
bool InputManager::bRelease[256] = {};

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

	if (FAILED(Keyboard->Acquire()))
	{
		tassert(true);
		return false;
	}

	return true;
}

void InputManager::Release()
{
	if (Mouse != nullptr)
	{
		Mouse->Unacquire();
		Mouse->Release();
		Mouse = nullptr;
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

	for (int i = 0; i < 256; ++i)
	{
		if (Key[i] & 0x80)
		{
			if (bPush[i] == false && bPress[i] == false)
			{
				bPress[i] = true;
			}
			else
			{
				bPress[i] = false;
				bPush[i] = true;
			}
		}
		else if (bPress[i] == true || bPush[i] == true)
		{
			bRelease[i] = true;
			bPress[i] = false;
			bPush[i] = false;
		}

		else if (bRelease[i] == true)
		{
			bRelease[i] = false;
		}
	}


#pragma region AxisKeyUpdate
	std::unordered_map<std::string, Ezptr<BindAxis>>::iterator AxisMapStartIter = AxisMap.begin();
	std::unordered_map<std::string, Ezptr<BindAxis>>::iterator AxisMapEndIter = AxisMap.end();

	for (; AxisMapStartIter != AxisMapEndIter ; ++AxisMapStartIter)
	{
		std::list<Ezptr<KeyScale>>::iterator KeyListStartIter = AxisMapStartIter->second->KeyList.begin();
		std::list<Ezptr<KeyScale>>::iterator KeyListEndIter = AxisMapStartIter->second->KeyList.end();

		for (; KeyListStartIter != KeyListEndIter ; ++KeyListStartIter)
		{
			float Scale = 0.0f;

			if (Key[(*KeyListStartIter)->Key] & 0x80)
			{
				Scale = (*KeyListStartIter)->Scale;
			}
			else
			{
				continue;
			}

			if (AxisMapStartIter->second->FuncBind == true)
			{
				AxisMapStartIter->second->Func(Scale, _Time);
			}
		}
	}
#pragma endregion

#pragma region ActionKeyUpdate
	std::unordered_map<std::string, Ezptr<BindAction>>::iterator ActionKeyStartIter = ActionMap.begin();
	std::unordered_map<std::string, Ezptr<BindAction>>::iterator ActionKeyEndIter = ActionMap.end();

	const unsigned char SKEY[SKEY_END] = { DIK_LCONTROL, DIK_LALT, DIK_LSHIFT };
	bool SKEYSTATE[SKEY_END][KS_END] = {};

	// 조합키의 상태를 체크한다.
	for (int i = 0; i < SKEY_END; i++)
	{
		if (Key[SKEY[i]] & 0x80)
		{
			SKEYSTATE[i][KS_PRESS] = true;
			SKEYSTATE[i][KS_PUSH] = true;
		}

		else if (SKEYSTATE[i][KS_PRESS] == true || SKEYSTATE[i][KS_PUSH] == true)
		{
			SKEYSTATE[i][KS_RELEASE] = true;
			SKEYSTATE[i][KS_PRESS] = false;
			SKEYSTATE[i][KS_PUSH] = false;
		}

		else if (SKEYSTATE[i][KS_RELEASE] == true)
		{
			SKEYSTATE[i][KS_RELEASE] = false;
		}
	}

	for (; ActionKeyStartIter != ActionKeyEndIter ; ++ActionKeyStartIter)
	{
		std::list<Ezptr<ActionKeyInfo>>::iterator ActionKeyInfoStartIter = ActionKeyStartIter->second->KeyList.begin();
		std::list<Ezptr<ActionKeyInfo>>::iterator ActionKeyEndIter = ActionKeyStartIter->second->KeyList.end();

		for (; ActionKeyInfoStartIter != ActionKeyEndIter  ; ++ActionKeyInfoStartIter)
		{
			bool KeyEnable = true;

			// 조합키와 함께 눌러져야할 키의 상태를 체크한다.
			if (Key[(*ActionKeyInfoStartIter)->Key] & 0x80)
			{
				if ((*ActionKeyInfoStartIter)->KeyState[KS_PRESS] == false && (*ActionKeyInfoStartIter)->KeyState[KS_PUSH] == false)
				{
					(*ActionKeyInfoStartIter)->KeyState[KS_PRESS] = true;
				}
				else
				{
					(*ActionKeyInfoStartIter)->KeyState[KS_PRESS] = false;
					(*ActionKeyInfoStartIter)->KeyState[KS_PUSH] = true;
				}
			}

			else if ((*ActionKeyInfoStartIter)->KeyState[KS_PRESS] == true || (*ActionKeyInfoStartIter)->KeyState[KS_PUSH] == true)
			{
				(*ActionKeyInfoStartIter)->KeyState[KS_RELEASE] = true;
				(*ActionKeyInfoStartIter)->KeyState[KS_PRESS] = false;
				(*ActionKeyInfoStartIter)->KeyState[KS_PUSH] = false;
			}

			else if ((*ActionKeyInfoStartIter)->KeyState[KS_RELEASE] == true)
			{
				(*ActionKeyInfoStartIter)->KeyState[KS_RELEASE] = false;
			}

			// 지정해놓은 키 조합이 있다면
			if ((*ActionKeyInfoStartIter)->KeyState[ActionKeyStartIter->second->KS] == true)
			{
				for (int i = 0; i < SKEY_END; i++)
				{
					// 지정해놓은 조합키를 찾고
					if ((*ActionKeyInfoStartIter)->SKey[i] == true)
					{
						// 해당되는 키를 찾는다.
						if (false == SKEYSTATE[i][ActionKeyStartIter->second->KS])
						{
							KeyEnable = false;
							break;
						}
					}

					// 조합키는 하나도 안눌렸을 경우
					else if (true == SKEYSTATE[i][ActionKeyStartIter->second->KS])
					{
						KeyEnable = false;
						break;
					}
				}
			}

			else
				KeyEnable = false;

			if (ActionKeyStartIter->second->FuncBind == true && KeyEnable == true)
			{
				ActionKeyStartIter->second->Func(_Time);
			}
		}
	}
#pragma endregion
}

Ezptr<BindAxis> InputManager::FindAxis(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<BindAxis>>::iterator FindIter = AxisMap.find(_Name);

	if (FindIter == AxisMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

Ezptr<BindAction> InputManager::FindAction(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<BindAction>>::iterator FindIter = ActionMap.find(_Name);

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
			Keyboard->Acquire();
		}
		else
			return false;
	}

	return true;
}
