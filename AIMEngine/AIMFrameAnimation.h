#pragma once
#include "AIMComponent.h"
#include "AIMTexture.h"
#include "AIMMaterial.h"

typedef struct Engine_DLL _tagTextureCoord
{
	Vec2 Start;
	Vec2 End;
}TextureCoord, *PTextureCoord;


class Engine_DLL FrameAnimationClip : public RefCounter
{
public:
	std::string Name;
	FrameAnimationType Type;
	AnimationOption Option;
	int Frame = 0.0f;
	Vec2 TextureSize;
	std::vector<TextureCoord> CoordVec;
	float PlayTime = 0.0f;
	float PlayLimitTime = 0.0f;
	float FrameTime = 0.0;
	Ezptr<AIMTexture> Texture;
	int Register = 0;
};

class Engine_DLL AIMFrameAnimation : public AIMComponent
{
	friend class AIMObject;

private:
	std::unordered_map<std::string, Ezptr<FrameAnimationClip>> ClipMap;
	Ezptr<FrameAnimationClip> CurClip;
	Ezptr<FrameAnimationClip> DefaultClip;
	FrameAnimationConstBuffer CBuffer;
	Ezptr<AIMMaterial> Material;

public:
	bool CreateClip(const std::string& _Name, AnimationOption _Option, const Vec2& _TextureSize, const std::vector<TextureCoord>& _CoordVec, float _LimitTime, int _Reg, const std::string& _TexName, const TCHAR* _FileName, const std::string& _Path = "Texture");
	bool CreateClip(const std::string& _Name, AnimationOption _Option, const Vec2& _TextureSize, const std::vector<TextureCoord>& _CoordVec, float _LimitTime, int _Reg, const std::string& _TexName, const std::vector<TCHAR*>& _FileNameVec, const std::string& _Path = "Texture");
	void SetCurrentClip(const std::string& _Name);
	void SetDefaultClip(const std::string& _Name);
	void ChangeClip(const std::string& _Name);

private:
	Ezptr<FrameAnimationClip> FindClip(const std::string& _Name);

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMFrameAnimation* Clone() const;

protected:
	AIMFrameAnimation();
	AIMFrameAnimation(const AIMFrameAnimation& _Other);
	~AIMFrameAnimation();
};

