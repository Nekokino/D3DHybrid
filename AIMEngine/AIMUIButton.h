#pragma once
#include "AIMUI.h"

enum ButtonState
{
	BS_DISABLE,
	BS_NORMAL,
	BS_MOUSEON,
	BS_CLICK,
	BS_END,
};

class AIMTexture;
class AIMMaterial;
class Engine_DLL AIMUIButton : public AIMUI
{
	friend class AIMObject;

protected:
	ButtonState PrevState = BS_NORMAL;
	ButtonState State = BS_NORMAL;
	bool StartTextureChange = true;
	std::function<void(float)> CallBack[BS_END];
	Ezptr<AIMTexture> Texture[BS_END];
	Vec4 Color[BS_END];
	Ezptr<AIMMaterial> Material;
	ButtonConstBuffer CBuffer;

public:
	void SetStateColor(ButtonState _State, const Vec4& _Color);
	void SetStateTexture(ButtonState _State, const std::string& _Name, const TCHAR* _FileName, const std::string& _Path = "Texture");
	void SetCallBack(ButtonState _State, void(*_Func)(float));

	template<typename T>
	void SetCallBack(ButtonState _State, T* _Obj, void(T::*_Func)(float))
	{
		CallBack[_State] = std::bind(_Func, _Obj, std::placeholders::_1);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMUIButton* Clone() const;

protected:
	AIMUIButton();
	AIMUIButton(const AIMUIButton& _Other);
	virtual ~AIMUIButton();
};

