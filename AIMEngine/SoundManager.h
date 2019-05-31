#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"
#include "fmod.hpp"
#include "AIMScene.h"

#ifdef _M_IX86
	#pragma comment(lib, "fmod_vc")
#else
	#pragma comment(lib, "fmod64_vc")
#endif

class Engine_DLL SoundInfo : public RefCounter
{
public:
	FMOD::Sound* Sound = nullptr;
	bool Loop = false;
	AIMScene* Scene;
	std::list<FMOD::Channel*> ChannelList;

	~SoundInfo()
	{
		int a = 0;
	}
};

class Engine_DLL SoundManager
{
private:
	static FMOD::System* System;
	static std::unordered_map<std::string, Ezptr<SoundInfo>> SoundMap;

public:
	static FMOD::System* GetSoundSystem();

public:
	static bool Init();
	static bool Release();
	static bool LoadSound(const std::string& _Name, AIMScene* _Scene, bool _Loop, const char* FileName, const std::string& _Path = "Sound");
	static void Play(const std::string& _Name);
	static void Play(Ezptr<SoundInfo> _Sound);
	static void Stop(const std::string& _Name);
	static void Stop(Ezptr<SoundInfo> _Sound);
	static void Update(float _Time);
	static void DeleteSound(AIMScene* _Scene);
	static Ezptr<SoundInfo> FindSound(const std::string& _Name);

private:
	SoundManager() {}
	virtual ~SoundManager() = 0 {}
};

