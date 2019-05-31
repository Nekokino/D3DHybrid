#include "AIMFrameAnimation.h"
#include "AIMTexture.h"
#include "ResourceManager.h"
#include "AIMTransform.h"
#include "AIMMaterial.h"
#include "AIMObject.h"
#include "ShaderManager.h"


AIMFrameAnimation::AIMFrameAnimation()
{
	CT = CT_FRAMEANIMATION;
}

AIMFrameAnimation::AIMFrameAnimation(const AIMFrameAnimation & _Other) : AIMComponent(_Other)
{
	CBuffer = _Other.CBuffer;

	std::unordered_map<std::string, Ezptr<FrameAnimationClip>>::const_iterator StartIter = _Other.ClipMap.begin();
	std::unordered_map<std::string, Ezptr<FrameAnimationClip>>::const_iterator EndIter = _Other.ClipMap.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		Ezptr<FrameAnimationClip> Clip = new FrameAnimationClip;

		Clip = StartIter->second;

		if (StartIter->first == _Other.CurClip->Name)
		{
			CurClip = Clip;
		}

		if (StartIter->first == _Other.DefaultClip->Name)
		{
			DefaultClip = Clip;
		}

		ClipMap.insert(std::unordered_map<std::string, Ezptr<FrameAnimationClip>>::value_type(StartIter->first, Clip));
	}

	Material = nullptr;
}

AIMFrameAnimation::~AIMFrameAnimation()
{
	ClipMap.clear();
}

bool AIMFrameAnimation::CreateClip(const std::string & _Name, AnimationOption _Option, const Vec2 & _TextureSize, const std::vector<TextureCoord>& _CoordVec, float _LimitTime, int _Reg, const std::string & _TexName, const TCHAR * _FileName, const std::string & _Path)
{
	Ezptr<FrameAnimationClip> Clip = FindClip(_Name);

	if (Clip != nullptr)
	{
		return false;
	}

	Clip = new FrameAnimationClip;

	Clip->Name = _Name;
	Clip->Type = AFT_ATLAS;
	Clip->Option = _Option;
	Clip->Frame = 0;
	Clip->PlayTime = 0.0f;
	Clip->TextureSize = _TextureSize;
	Clip->CoordVec = _CoordVec;
	Clip->PlayLimitTime = _LimitTime;
	Clip->FrameTime = _LimitTime / _CoordVec.size();
	Clip->Register = _Reg;
	ResourceManager::LoadTexture(_TexName, _FileName, _Path);
	Clip->Texture = ResourceManager::FindTexture(_TexName);

	if (CurClip == nullptr)
	{
		CurClip = Clip;
	}

	if (DefaultClip == nullptr)
	{
		DefaultClip = Clip;
	}

	ClipMap.insert(std::unordered_map<std::string, Ezptr<AIMFrameAnimation>>::value_type(_Name, Clip));

	return true;
}

bool AIMFrameAnimation::CreateClip(const std::string & _Name, AnimationOption _Option, const Vec2 & _TextureSize, const std::vector<TextureCoord>& _CoordVec, float _LimitTime, int _Reg, const std::string & _TexName, const std::vector<TCHAR*>& _FileNameVec, const std::string & _Path)
{
	Ezptr<FrameAnimationClip> Clip = FindClip(_Name);

	if (Clip != nullptr)
	{
		return false;
	}

	Clip = new FrameAnimationClip;

	Clip->Name = _Name;
	Clip->Type = AFT_ATLAS;
	Clip->Option = _Option;
	Clip->Frame = 0;
	Clip->PlayTime = 0.0f;
	Clip->TextureSize = _TextureSize;
	Clip->CoordVec = _CoordVec;
	Clip->PlayLimitTime = _LimitTime;
	Clip->FrameTime = _LimitTime / _CoordVec.size();
	Clip->Register = _Reg;
	ResourceManager::LoadTexture(_TexName, _FileNameVec, _Path);
	Clip->Texture = ResourceManager::FindTexture(_TexName);

	if (CurClip == nullptr)
	{
		CurClip = Clip;
	}

	if (DefaultClip == nullptr)
	{
		DefaultClip = Clip;
	}

	ClipMap.insert(std::unordered_map<std::string, Ezptr<FrameAnimationClip>>::value_type(_Name, Clip));

	return true;
}

void AIMFrameAnimation::SetCurrentClip(const std::string & _Name)
{
	Ezptr<FrameAnimationClip> Clip = FindClip(_Name);

	if (Clip == nullptr)
	{
		return;
	}

	CurClip = Clip;

	CurClip->Frame = 0;
	CurClip->PlayTime = 0.0f;
}

void AIMFrameAnimation::SetDefaultClip(const std::string & _Name)
{
	Ezptr<FrameAnimationClip> Clip = FindClip(_Name);

	if (Clip == nullptr)
	{
		return;
	}

	DefaultClip = Clip;
}

void AIMFrameAnimation::ChangeClip(const std::string & _Name)
{
	if (CurClip->Name == _Name)
	{
		return;
	}

	std::string PrevName = CurClip->Texture->GetNameTag();

	SetCurrentClip(_Name);

	Material->ChangeTextureSet(0, 0, CurClip->Register, PrevName, CurClip->Texture);
}

Ezptr<FrameAnimationClip> AIMFrameAnimation::FindClip(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<FrameAnimationClip>>::iterator FindIter = ClipMap.find(_Name);

	if (FindIter == ClipMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void AIMFrameAnimation::Start()
{
	Material = FindComponent<AIMMaterial>(CT_MATERIAL);

	if (Material == nullptr)
	{
		Material = Object->AddComponent<AIMMaterial>("Material");
	}

	Material->DeleteTextureSet(0, 0);

	Material->AddTextureSet(0, 0, CurClip->Register, CurClip->Texture);
}

bool AIMFrameAnimation::Init()
{
	return true;
}

int AIMFrameAnimation::Input(float _Time)
{
	return 0;
}

int AIMFrameAnimation::Update(float _Time)
{
	CurClip->PlayTime += _Time;

	while (CurClip->PlayTime >= CurClip->FrameTime)
	{
		CurClip->PlayTime -= CurClip->FrameTime;

		++CurClip->Frame;

		if (CurClip->Frame == CurClip->CoordVec.size())
		{
			CurClip->Frame = 0;

			switch (CurClip->Option)
			{
			case AO_ONCE_DESTROY:
				Object->SetAlive(false);
				break;
			}
		}
	}

	CBuffer.FrameAnimationType = CurClip->Type;
	CBuffer.AnimationOption = CurClip->Option;
	CBuffer.Frame = CurClip->Frame;
	CBuffer.TextureSize = CurClip->TextureSize;
	CBuffer.Start = CurClip->CoordVec[CurClip->Frame].Start;
	CBuffer.End = CurClip->CoordVec[CurClip->Frame].End;

	return 0;
}

int AIMFrameAnimation::LateUpdate(float _Time)
{
	return 0;
}

int AIMFrameAnimation::Collision(float _Time)
{
	return 0;
}

int AIMFrameAnimation::PrevRender(float _Time)
{
	ShaderManager::UpdateConstBuffer("FrameAnimation", &CBuffer);

	return 0;
}

int AIMFrameAnimation::Render(float _Time)
{
	return 0;
}

AIMFrameAnimation * AIMFrameAnimation::Clone() const
{
	return new AIMFrameAnimation(*this);
}
