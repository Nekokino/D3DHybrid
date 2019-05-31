#pragma once

#include "AIMComponent.h"

class BoneSocket;
class Engine_DLL Bone : public RefCounter
{
public:
	std::string Name;
	int Depth = 0;
	int ParendIndex = 0;
	Matrix* OffsetMat = nullptr;
	Matrix* BoneMat = nullptr;
	//std::list<Ezptr<BoneSocket>> SocketList;

	~Bone()
	{
		if (OffsetMat != nullptr)
		{
			delete OffsetMat;
			OffsetMat = nullptr;
		}

		if (BoneMat != nullptr)
		{
			delete BoneMat;
			BoneMat = nullptr;
		}
	}
};

class Engine_DLL KeyFrame : public RefCounter
{
public:
	double Time = 0.0;
	Vec3 Pos;
	Vec3 Scale;
	Vec4 Rot;
};

class Engine_DLL BoneKeyFrame : public RefCounter
{
public:
	int BoneIndex = 0;
	std::vector<Ezptr<KeyFrame>> KeyFrameVec;
};

class Engine_DLL AnimationCallback : public RefCounter
{
public:
	int iAnimationProgress = 0;
	float fAnimationProgress = 0.0f;
	std::function<void(float)> Func;
	bool bCall = false;
};

class Engine_DLL AnimationClip : public RefCounter
{
public:
	AnimationOption Option = AO_LOOP;
	std::string Name = "";
	float StartTime = 0.0f;
	float EndTime = 0.0f;
	float TimeLength = 0.0f;
	float FrameTime = 0.0f;
	float PlayTime = 10.0f;
	int StartFrame = 0;
	int EndFrame = 0;
	int FrameLength = 0;
	int FrameMode = 0;
	int ChangeFrame = 0;
	std::vector<Ezptr<BoneKeyFrame>> KeyFrameVec;
	std::vector<Ezptr<AnimationCallback>> CallBackVec;

	void GetData(const AnimationClip& _Data)
	{
		Option = _Data.Option;
		Name = _Data.Name;
		StartTime = _Data.StartTime;
		EndTime = _Data.EndTime;
		TimeLength = _Data.TimeLength;
		FrameTime = _Data.FrameTime;
		PlayTime = _Data.PlayTime;
		StartFrame = _Data.StartFrame;
		EndFrame = _Data.EndFrame;
		FrameLength = _Data.FrameLength;
		FrameMode = _Data.FrameMode;
		ChangeFrame = _Data.ChangeFrame;
		KeyFrameVec = _Data.KeyFrameVec;
		CallBackVec = _Data.CallBackVec;
	}

	~AnimationClip()
	{
		KeyFrameVec.clear();
		CallBackVec.clear();
	}
};

typedef struct Engine_DLL _tagBlendInfo
{
	Vec3 Scale;
	Vec3 Pos;
	Vec3 Rot;
}BlendInfo, *PBlendInfo;

class FbxAnimationClip;
class Engine_DLL AIMAnimation : public AIMComponent
{
	friend class AIMObject;
	friend class AIMMesh;

private:
	std::vector<Ezptr<Bone>> BoneVec;
	ID3D11Texture2D* BoneTex = nullptr;
	ID3D11ShaderResourceView* BoneSRV = nullptr;
	std::vector<Matrix*> BoneMatrixVec;
	std::unordered_map<std::string, Ezptr<AnimationClip>> ClipMap;
	Ezptr<AnimationClip> DefaultClip;
	Ezptr<AnimationClip> CurClip;
	Ezptr<AnimationClip> NextClip;
	std::list<std::string> AddClipNameList;
	bool End = false;
	float AnimationGlobalTime = 0.0f;
	float ClipProgress = 0.0f;
	float ChangeTime = 0.0f;
	float ChangeLimitTime = 0.25f;
	std::vector<BlendInfo> BlendInfoVec;

public:
	const std::list<std::string>* GetAddClipList() const;
	void AddBone(Ezptr<Bone> _Bone);
	bool CreateBoneTexture();
	void AddClip(AnimationOption _Option, Ezptr<FbxAnimationClip> _Clip);
	void AddClip(const std::string& _Name, AnimationOption _Option, int _StartFrame, int _EndFrame, float _PlayTime, const std::vector<Ezptr<BoneKeyFrame>>& _Frame);
	void AddClip(const TCHAR* _FullPath);
	void AddClipFromMultibyte(const char* _FullPath);
	Ezptr<AnimationClip> FindClip(const std::string& _Name);
	bool IsAnimationEnd() const;
	Ezptr<AnimationClip> GetCurrentClip() const;
	const std::unordered_map<std::string, Ezptr<AnimationClip>>* GetClips() const;
	void GetCurrentKeyFrame(std::vector<Ezptr<BoneKeyFrame>>& _KeyFrameVec);

public:
	void ChangeClipName(const std::string& _Origin, const std::string& _Change);
	Ezptr<Bone> FindBone(const std::string& _Name);
	int FindBoneIndex(const std::string& _Name);
	Matrix GetBoneMatrix(const std::string& _Name);
	bool ChangeClip(const std::string& _Name);
	ID3D11ShaderResourceView** GetBoneSRV();
	bool Save(const TCHAR* _FileName, const std::string& _Path = "Mesh");
	bool Save(const char* _FileName, const std::string& _Path = "Mesh");
	bool SaveFromFullPath(const TCHAR* _FullPath);
	bool SaveFromFullPath(const char* _FullPath);
	bool Load(const TCHAR* _FileName, const std::string& _Path = "Mesh");
	bool Load(const char* _FileName, const std::string& _Path = "Mesh");
	bool LoadFromFullPath(const TCHAR* _FullPath);
	bool LoadFromFullPath(const char* _FullPath);

	bool SaveBone(const TCHAR* _FileName, const std::string& _Path = "Mesh");
	bool SaveBone(const char* _FileName, const std::string& _Path = "Mesh");
	bool SaveBoneFromFullPath(const TCHAR* _FullPath);
	bool SaveBoneFromFullPath(const char* _FullPath);
	bool LoadBone(const TCHAR* _FileName, const std::string& _Path = "Mesh");
	bool LoadBone(const char* _FileName, const std::string& _Path = "Mesh");
	bool LoadBoneFromFullPath(const TCHAR* _FullPath);
	bool LoadBoneFromFullPath(const char* _FullPath);

	bool ModifyClip(const std::string& _Name, const std::string& _Change, AnimationOption _Option, int _StartFrame, int _EndFrame, float _PlayTime, const std::vector<Ezptr<BoneKeyFrame>>& _KeyFrameVec);
	bool DeleteClip(const std::string& _Name);
	bool ReturnDefaultClip();

private:
	void LoadFbxAnimation(const char* _FullPath);

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMAnimation* Clone() const;

protected:
	AIMAnimation();
	AIMAnimation(const AIMAnimation& _Other);
	~AIMAnimation();
};

