#pragma once
#include "AIMComponent.h"
#include "SoundManager.h"


class Engine_DLL AIMSound : public AIMComponent
{
	friend class AIMObject;

private:
	std::string FileName;
	Ezptr<SoundInfo> Info;
	FMOD::Channel* Channel = nullptr;
	bool IsBGM = false;
	bool IsLoop = false;
	bool IsPlay = false;

public:
	void SetSound(const std::string& _Name);
	void SetSound(const std::string& _Name, Ezptr<AIMScene> _Scene, bool _Loop, const char* _FileName, const std::string& _Path = "Sound");
	void EnableBGM();
	void Play();
	void Stop();
	void Pause();


public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMSound* Clone() const;

protected:
	AIMSound();
	AIMSound(const AIMSound& _Other);
	~AIMSound();
};

