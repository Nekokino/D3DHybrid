#include "AIMSound.h"



AIMSound::AIMSound()
{
	CT = CT_SOUND;
}

AIMSound::AIMSound(const AIMSound & _Other) : AIMComponent(_Other)
{
	FileName = _Other.FileName;
	Channel = _Other.Channel;
	Info = _Other.Info;
	IsBGM = _Other.IsBGM;
	IsLoop = _Other.IsLoop;
	IsPlay = _Other.IsPlay;
}


AIMSound::~AIMSound()
{

}

void AIMSound::SetSound(const std::string & _Name)
{
	Info = SoundManager::FindSound(_Name);
}

void AIMSound::SetSound(const std::string & _Name, Ezptr<AIMScene> _Scene, bool _Loop, const char* _FileName, const std::string & _Path)
{
	if (SoundManager::LoadSound(_Name, _Scene, _Loop, _FileName, _Path) == false)
	{
		return;
	}

	Info = SoundManager::FindSound(_Name);
}

void AIMSound::EnableBGM()
{
	IsBGM = true;
}

void AIMSound::Play()
{
	if (IsPlay == true)
	{
		Channel->stop();

		std::list<FMOD::Channel*>::iterator StartIter = Info->ChannelList.begin();
		std::list<FMOD::Channel*>::iterator EndIter = Info->ChannelList.end();

		for (; StartIter != EndIter ; ++StartIter)
		{
			if ((*StartIter) == Channel)
			{
				Info->ChannelList.erase(StartIter);
				break;
			}
		}

		Channel = nullptr;
	}

	IsPlay = true;

	SoundManager::Play(Info);

	Channel = Info->ChannelList.back();
}

void AIMSound::Stop()
{
	if (IsPlay == true)
	{
		Channel->stop();

		std::list<FMOD::Channel*>::iterator StartIter = Info->ChannelList.begin();
		std::list<FMOD::Channel*>::iterator EndIter = Info->ChannelList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if ((*StartIter) == Channel)
			{
				Info->ChannelList.erase(StartIter);
				break;
			}
		}
	}
}

void AIMSound::Pause()
{
}

void AIMSound::Start()
{
	if (IsPlay == true)
	{
		if (Channel != nullptr)
		{
			Channel->stop();

			std::list<FMOD::Channel*>::iterator StartIter = Info->ChannelList.begin();
			std::list<FMOD::Channel*>::iterator EndIter = Info->ChannelList.end();

			for (; StartIter != EndIter; ++StartIter)
			{
				if ((*StartIter) == Channel)
				{
					Info->ChannelList.erase(StartIter);
					break;
				}
			}

			Channel = nullptr;
		}

		SoundManager::Play(Info);
	
		Channel = Info->ChannelList.back();
	}
}

bool AIMSound::Init()
{
	return true;
}

int AIMSound::Input(float _Time)
{
	return 0;
}

int AIMSound::Update(float _Time)
{
	return 0;
}

int AIMSound::LateUpdate(float _Time)
{
	return 0;
}

int AIMSound::Collision(float _Time)
{
	return 0;
}

int AIMSound::PrevRender(float _Time)
{
	return 0;
}

int AIMSound::Render(float _Time)
{
	return 0;
}

AIMSound * AIMSound::Clone() const
{
	return new AIMSound(*this);
}
