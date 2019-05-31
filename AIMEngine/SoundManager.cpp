#include "SoundManager.h"
#include "PathManager.h"


FMOD::System* SoundManager::System = nullptr;
std::unordered_map<std::string, Ezptr<SoundInfo>> SoundManager::SoundMap;

FMOD::System * SoundManager::GetSoundSystem()
{
	return System;
}

bool SoundManager::Init()
{
	FMOD::System_Create(&System);

	System->init(FMOD_MAX_CHANNEL_WIDTH, FMOD_INIT_NORMAL, nullptr);

	return true;
}

bool SoundManager::Release()
{
	std::unordered_map<std::string, Ezptr<SoundInfo>>::iterator StartIter = SoundMap.begin();
	std::unordered_map<std::string, Ezptr<SoundInfo>>::iterator EndIter = SoundMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		StartIter->second->Sound->release();
	}

	SoundMap.clear();

	if (System != nullptr)
	{
		System->close();
		System->release();
	}

	return true;
}

bool SoundManager::LoadSound(const std::string & _Name,AIMScene* _Scene, bool _Loop, const char * _FileName, const std::string & _Path)
{
	Ezptr<SoundInfo> Info = FindSound(_Name);

	if (Info != nullptr)
	{
		return false;
	}

	Info = new SoundInfo;

	Info->Loop = _Loop;

	const char* Path = PathManager::FindPathMultiByte(_Path);

	std::string TmpPath;

	if (Path != nullptr)
	{
		TmpPath = Path;
	}

	TmpPath += _FileName;

	FMOD_MODE Mode = FMOD_LOOP_NORMAL;

	if (_Loop == false)
	{
		Mode = FMOD_DEFAULT;
	}

	if (System->createSound(TmpPath.c_str(), Mode, nullptr, &Info->Sound) != FMOD_OK)
	{
		return false;
	}

	Info->Scene = _Scene;

	SoundMap.insert(std::unordered_map<std::string, Ezptr<SoundInfo>>::value_type(_Name, Info));

	return true;
}

void SoundManager::Play(const std::string & _Name)
{
	Ezptr<SoundInfo> Info = FindSound(_Name);

	if (Info == nullptr)
	{
		return;
	}

	FMOD::Channel* Channel = nullptr;

	System->playSound(Info->Sound, nullptr, false, &Channel);

	Info->ChannelList.push_back(Channel);
}

void SoundManager::Play(Ezptr<SoundInfo> _Sound)
{
	FMOD::Channel* Channel = nullptr;

	System->playSound(_Sound->Sound, nullptr, false, &Channel);

	_Sound->ChannelList.push_back(Channel);
}

void SoundManager::Stop(const std::string & _Name)
{
	Ezptr<SoundInfo> Info = FindSound(_Name);

	if (Info == nullptr)
	{
		return;
	}

	std::list<FMOD::Channel*>::iterator StartIter = Info->ChannelList.begin();
	std::list<FMOD::Channel*>::iterator EndIter = Info->ChannelList.end();

	for (; StartIter != EndIter  ; ++StartIter)
	{
		(*StartIter)->stop();
	}

	Info->ChannelList.clear();
}

void SoundManager::Stop(Ezptr<SoundInfo> _Sound)
{
	std::list<FMOD::Channel*>::iterator StartIter = _Sound->ChannelList.begin();
	std::list<FMOD::Channel*>::iterator EndIter = _Sound->ChannelList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->stop();
	}

	_Sound->ChannelList.clear();
}

void SoundManager::Update(float _Time)
{
	std::unordered_map<std::string, Ezptr<SoundInfo>>::iterator StartIter = SoundMap.begin();
	std::unordered_map<std::string, Ezptr<SoundInfo>>::iterator EndIter = SoundMap.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		std::list<FMOD::Channel*>::iterator StartIter2 = StartIter->second->ChannelList.begin();
		std::list<FMOD::Channel*>::iterator EndIter2 = StartIter->second->ChannelList.end();

		for (; StartIter2 != EndIter2 ;)
		{
			bool IsPlay = false;
			(*StartIter2)->isPlaying(&IsPlay);

			if (IsPlay == false)
			{
				StartIter2 = StartIter->second->ChannelList.erase(StartIter2);
			}
			else
			{
				++StartIter2;
			}
		}
	}

}

void SoundManager::DeleteSound(AIMScene* _Scene)
{
	std::unordered_map<std::string, Ezptr<SoundInfo>>::iterator StartIter = SoundMap.begin();
	std::unordered_map<std::string, Ezptr<SoundInfo>>::iterator EndIter = SoundMap.end();

	for (; StartIter != EndIter;)
	{
		if (StartIter->second->Scene == _Scene)
		{
			StartIter->second->Sound->release();

			if (StartIter->second != nullptr)
			{
				StartIter = SoundMap.erase(StartIter);
			}
		}
		else
		{
			++StartIter;
		}

	}

}

Ezptr<SoundInfo> SoundManager::FindSound(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<SoundInfo>>::iterator FindIter = SoundMap.find(_Name);

	if (FindIter == SoundMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}
