#include "AIMAnimation.h"
#include "AIMDevice.h"
#include "FbxLoader.h"
#include "AIMObject.h"
#include "PathManager.h"

AIMAnimation::AIMAnimation()
{
	CT = CT_ANIMATION;
}

AIMAnimation::AIMAnimation(const AIMAnimation & _Other) : AIMComponent(_Other)
{
	BoneVec = _Other.BoneVec;
	BoneMatrixVec = _Other.BoneMatrixVec;
	ClipMap = _Other.ClipMap;
	AnimationGlobalTime = _Other.AnimationGlobalTime;
	ClipProgress = _Other.ClipProgress;
	ChangeTime = _Other.ChangeTime;
	ChangeLimitTime = _Other.ChangeLimitTime;
	BlendInfoVec = _Other.BlendInfoVec;

	DefaultClip = nullptr;
	CurClip = nullptr;
	NextClip = nullptr;
	End = false;

	BoneTex = nullptr;
	BoneSRV = nullptr;

	CreateBoneTexture();

	BoneMatrixVec.clear();

	BoneMatrixVec.resize(_Other.BoneMatrixVec.size());

	for (size_t i = 0; i < BoneMatrixVec.size(); i++)
	{
		BoneMatrixVec[i] = new Matrix;
	}

	std::unordered_map<std::string, Ezptr<AnimationClip>>::const_iterator StartIter = _Other.ClipMap.begin();
	std::unordered_map<std::string, Ezptr<AnimationClip>>::const_iterator EndIter = _Other.ClipMap.end();

	ClipMap.clear();

	for (; StartIter != EndIter ; ++StartIter)
	{
		Ezptr<AnimationClip> Clip = new AnimationClip;

		Clip->GetData(*(StartIter->second));

		Clip->CallBackVec.clear();

		if (_Other.CurClip->Name == StartIter->first)
		{
			CurClip = Clip;
		}

		if (_Other.DefaultClip->Name == StartIter->first)
		{
			DefaultClip = Clip;
		}

		ClipMap.insert(std::unordered_map<std::string, Ezptr<AnimationClip>>::value_type(StartIter->first, Clip));
	}
}


AIMAnimation::~AIMAnimation()
{
	if (BoneTex != nullptr)
	{
		BoneTex->Release();
		BoneTex = nullptr;
	}

	if (BoneSRV != nullptr)
	{
		BoneSRV->Release();
		BoneSRV = nullptr;
	}

	for (size_t i = 0; i < BoneMatrixVec.size(); i++)
	{
		if (BoneMatrixVec[i] != nullptr)
		{
			delete BoneMatrixVec[i];
			BoneMatrixVec[i] = nullptr;
		}
	}

	BoneVec.clear();
	ClipMap.clear();
}

const std::list<std::string>* AIMAnimation::GetAddClipList() const
{
	return &AddClipNameList;
}

void AIMAnimation::AddBone(Ezptr<Bone> _Bone)
{
	BoneVec.push_back(_Bone);
}

bool AIMAnimation::CreateBoneTexture()
{
	if (BoneTex != nullptr)
	{
		BoneTex->Release();
		BoneTex = nullptr;
	}

	D3D11_TEXTURE2D_DESC Desc = {};
	Desc.ArraySize = 1;
	Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	Desc.Height = 1;
	Desc.Width = (UINT)(BoneVec.size() * 4);
	Desc.MipLevels = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.SampleDesc.Count = 1;

	if (FAILED(GetAIMDevice->CreateTexture2D(&Desc, NULL, &BoneTex)))
	{
		tassertmsg(true, "Failed To Create Texture2D");
		return false;
	}

	if (FAILED(GetAIMDevice->CreateShaderResourceView(BoneTex, NULL, &BoneSRV)))
	{
		tassertmsg(true, "Failed To Create ShaderResourceView");
		return false;
	}

	BlendInfoVec.resize(BoneVec.size());

	return true;
}

void AIMAnimation::AddClip(AnimationOption _Option, Ezptr<FbxAnimationClip> _Clip)
{
	Ezptr<AnimationClip> AnimClip = FindClip(_Clip->Name);

	if (AnimClip != nullptr)
	{
		tassertmsg(true, "Overlapped Animation Clip Name");
		return;
	}

	AnimClip = new AnimationClip;

	AnimClip->Option = _Option;
	AnimClip->Name = _Clip->Name;

	AnimClip->ChangeFrame = 0;

	AnimClip->StartFrame = (int)_Clip->Start.GetFrameCount(_Clip->TimeMode);
	AnimClip->EndFrame = (int)_Clip->End.GetFrameCount(_Clip->TimeMode);
	AnimClip->FrameLength = AnimClip->EndFrame - AnimClip->StartFrame;

	AnimClip->StartTime = 0.0f;
	AnimClip->EndTime = AnimClip->PlayTime;
	AnimClip->TimeLength = AnimClip->PlayTime;
	
	AnimClip->FrameTime = AnimClip->PlayTime / AnimClip->FrameLength;

	for (size_t AA = 0; AA < _Clip->BoneKeyFrameVec.size(); AA++)
	{
		Ezptr<BoneKeyFrame> TmpBoneKeyFrame = new BoneKeyFrame;

		TmpBoneKeyFrame->BoneIndex = _Clip->BoneKeyFrameVec[AA].BoneIndex;

		AnimClip->KeyFrameVec.push_back(TmpBoneKeyFrame);

		TmpBoneKeyFrame->KeyFrameVec.reserve(_Clip->BoneKeyFrameVec[AA].KeyFrameVec.size());

		for (size_t BB = 0; BB < _Clip->BoneKeyFrameVec[AA].KeyFrameVec.size(); BB++)
		{
			Ezptr<KeyFrame> TmpKeyFrame = new KeyFrame;

			TmpKeyFrame->Time = BB * AnimClip->FrameTime;

			FbxAMatrix Mat = _Clip->BoneKeyFrameVec[AA].KeyFrameVec[BB].TransformMat;

			FbxVector4 Pos, Scale;
			FbxQuaternion Rot;

			Pos = Mat.GetT();
			Scale = Mat.GetS();
			Rot = Mat.GetQ();

			TmpKeyFrame->Scale = Vec3((float)Scale.mData[0], (float)Scale.mData[1], (float)Scale.mData[2]);
			TmpKeyFrame->Pos = Vec3((float)Pos.mData[0], (float)Pos.mData[1], (float)Pos.mData[2]);
			TmpKeyFrame->Rot = Vec4((float)Rot.mData[0], (float)Rot.mData[1], (float)Rot.mData[2], (float)Rot.mData[3]);

			TmpBoneKeyFrame->KeyFrameVec.push_back(TmpKeyFrame);
		}
	}

	switch (_Clip->TimeMode)
	{
	case FbxTime::eFrames24:
		AnimClip->FrameMode = 24;
		break;
	case FbxTime::eFrames30:
		AnimClip->FrameMode = 30;
		break;
	case FbxTime::eFrames60:
		AnimClip->FrameMode = 60;
		break;
	}

	if (ClipMap.empty() == true)
	{
		DefaultClip = AnimClip;
		CurClip = AnimClip;
	}

	ClipMap.insert(std::unordered_map<std::string, Ezptr<AnimationClip>>::value_type(AnimClip->Name, AnimClip));

	AddClipNameList.clear();

	AddClipNameList.push_back(AnimClip->Name);

	AnimClip->FrameTime = AnimClip->PlayTime / AnimClip->FrameLength;

	if (ClipMap.empty() == true)
	{
		DefaultClip = AnimClip;
		CurClip = AnimClip;
	}

	ClipMap.insert(std::unordered_map<std::string, Ezptr<AnimationClip>>::value_type(AnimClip->Name, AnimClip));
}

void AIMAnimation::AddClip(const std::string & _Name, AnimationOption _Option, int _StartFrame, int _EndFrame, float _PlayTime, const std::vector<Ezptr<BoneKeyFrame>>& _KeyFrameVec)
{
	Ezptr<AnimationClip> AnimClip = FindClip(_Name);

	if (AnimClip != nullptr)
	{
		return;
	}

	AnimClip = new AnimationClip;

	AnimClip->Option = _Option;
	AnimClip->Name = _Name;

	AnimClip->ChangeFrame = 0;

	int Length = _EndFrame - _StartFrame;

	AnimClip->StartFrame = 0;
	AnimClip->EndFrame = Length;
	AnimClip->FrameLength = Length;

	AnimClip->StartTime = 0.0f;
	AnimClip->EndTime = _PlayTime;
	AnimClip->TimeLength = _PlayTime;
	AnimClip->PlayTime = _PlayTime;

	AnimClip->FrameTime = AnimClip->PlayTime / AnimClip->FrameLength;

	for (size_t i = 0; i < _KeyFrameVec.size(); i++)
	{
		Ezptr<BoneKeyFrame> BoneFrame = new BoneKeyFrame;

		AnimClip->KeyFrameVec.push_back(BoneFrame);

		Ezptr<BoneKeyFrame> ClipFrame = _KeyFrameVec[i];

		BoneFrame->BoneIndex = ClipFrame->BoneIndex;

		if (ClipFrame->KeyFrameVec.empty() == false)
		{
			for (size_t BB = _StartFrame; BB <= _EndFrame; BB++)
			{
				Ezptr<KeyFrame> Frame = new KeyFrame;

				Frame->Time = (BB - _StartFrame) * AnimClip->FrameTime;
				Frame->Pos = ClipFrame->KeyFrameVec[BB]->Pos;
				Frame->Scale = ClipFrame->KeyFrameVec[BB]->Scale;
				Frame->Rot = ClipFrame->KeyFrameVec[BB]->Rot;

				BoneFrame->KeyFrameVec.push_back(Frame);
			}
		}
	}

	if (ClipMap.empty() == true)
	{
		DefaultClip = AnimClip;
		CurClip = AnimClip;
	}

	ClipMap.insert(std::unordered_map<std::string, Ezptr<AnimationClip>>::value_type(_Name, AnimClip));

	AddClipNameList.clear();
	AddClipNameList.push_back(AnimClip->Name);

}

void AIMAnimation::AddClip(const TCHAR * _FullPath)
{
	char FullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, _FullPath, -1, FullPath, lstrlen(_FullPath), 0, 0);
#else
	strcpy_s(Fullpath, _FullPath);
#endif

	AddClipFromMultibyte(FullPath);
}

void AIMAnimation::AddClipFromMultibyte(const char * _FullPath)
{
	char Ext[_MAX_EXT] = {};

	_splitpath_s(_FullPath, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);

	_strupr_s(Ext);

	if (strcmp(Ext, ".FBX") == 0)
	{
		LoadFbxAnimation(_FullPath);
	}
	else
	{
		LoadFromFullPath(_FullPath);
	}
}

Ezptr<AnimationClip> AIMAnimation::FindClip(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<AnimationClip>>::iterator FindIter = ClipMap.find(_Name);

	if (FindIter == ClipMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

bool AIMAnimation::IsAnimationEnd() const
{
	return End;
}

Ezptr<AnimationClip> AIMAnimation::GetCurrentClip() const
{
	return CurClip;
}

const std::unordered_map<std::string, Ezptr<AnimationClip>>* AIMAnimation::GetClips() const
{
	return &ClipMap;
}

void AIMAnimation::GetCurrentKeyFrame(std::vector<Ezptr<BoneKeyFrame>>& _KeyFrameVec)
{
	for (size_t AA = 0; AA < CurClip->KeyFrameVec.size(); AA++)
	{
		Ezptr<BoneKeyFrame> BoneFrame = new BoneKeyFrame;
		_KeyFrameVec.push_back(BoneFrame);

		Ezptr<BoneKeyFrame> ClipFrame = CurClip->KeyFrameVec[AA];
		BoneFrame->BoneIndex = ClipFrame->BoneIndex;
		
		for (size_t BB = 0; BB < ClipFrame->KeyFrameVec.size(); BB++)
		{
			Ezptr<KeyFrame> Frame = new KeyFrame;

			Frame->Time = ClipFrame->KeyFrameVec[BB]->Time;
			Frame->Pos = ClipFrame->KeyFrameVec[BB]->Pos;
			Frame->Rot = ClipFrame->KeyFrameVec[BB]->Rot;
			Frame->Scale = ClipFrame->KeyFrameVec[BB]->Scale;

			BoneFrame->KeyFrameVec.push_back(Frame);
		}
	}
}

void AIMAnimation::ChangeClipName(const std::string & _Origin, const std::string & _Change)
{
	std::unordered_map<std::string, Ezptr<AnimationClip>>::iterator FindIter = ClipMap.find(_Origin);

	if (FindIter == ClipMap.end())
	{
		return;
	}

	Ezptr<AnimationClip> Clip = FindIter->second;

	Clip->Name = _Change;

	ClipMap.erase(FindIter);

	ClipMap.insert(std::unordered_map<std::string, Ezptr<AnimationClip>>::value_type(_Change, Clip));
}

Ezptr<Bone> AIMAnimation::FindBone(const std::string & _Name)
{
	for (size_t i = 0; i < BoneVec.size(); i++)
	{
		if (BoneVec[i]->Name == _Name)
		{
			return BoneVec[i];
		}
	}

	return nullptr;
}

int AIMAnimation::FindBoneIndex(const std::string & _Name)
{
	for (size_t i = 0; i < BoneVec.size(); i++)
	{
		if (BoneVec[i]->Name == _Name)
		{
			return (int)i;
		}
	}

	return -1;
}

Matrix AIMAnimation::GetBoneMatrix(const std::string & _Name)
{
	int Bone = FindBoneIndex(_Name);

	if (Bone == -1)
	{
		return Matrix();
	}

	return *BoneMatrixVec[Bone];
}

bool AIMAnimation::ChangeClip(const std::string & _Name)
{
	if (CurClip->Name == _Name)
	{
		return false;
	}

	NextClip = FindClip(_Name);

	if (NextClip == nullptr)
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView ** AIMAnimation::GetBoneSRV()
{
	return &BoneSRV;
}

bool AIMAnimation::Save(const TCHAR * _FileName, const std::string & _Path)
{
	char FileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, _FileName, -1, FileName, lstrlen(_FileName), NULL, NULL);
#else
	strcpy_s(FileName, _FileName);
#endif

	return Save(FileName, _Path);
}

bool AIMAnimation::Save(const char * _FileName, const std::string & _Path)
{
	const char* Path = PathManager::FindPathMultiByte(_Path);

	std::string FullPath;

	if (Path != nullptr)
	{
		FullPath += Path;
	}

	FullPath += _FileName;

	return SaveFromFullPath(FullPath.c_str());
}

bool AIMAnimation::SaveFromFullPath(const TCHAR * _FullPath)
{
	char FileName[MAX_PATH] = {};
#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, _FullPath, -1, FileName, lstrlen(_FullPath), NULL, NULL);
#else
	strcpy_s(FileName, _FullPath);
#endif

	return SaveFromFullPath(FileName);
}

bool AIMAnimation::SaveFromFullPath(const char * _FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, _FullPath, "wb");

	if (File == nullptr)
	{
		tassertmsg(true, "Failed To Create File");
		return false;
	}

	fwrite(&ChangeLimitTime, sizeof(float), 1, File);

	size_t Count = ClipMap.size();

	fwrite(&Count, sizeof(size_t), 1, File);

	size_t Length = DefaultClip->Name.length();

	fwrite(&Length, sizeof(size_t), 1, File);
	fwrite(DefaultClip->Name.c_str(), sizeof(char), Length, File);

	Length = CurClip->Name.length();

	fwrite(&Length, sizeof(size_t), 1, File);
	fwrite(CurClip->Name.c_str(), sizeof(char), Length, File);

	std::unordered_map<std::string, Ezptr<AnimationClip>>::iterator StartIter = ClipMap.begin();
	std::unordered_map<std::string, Ezptr<AnimationClip>>::iterator EndIter = ClipMap.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		Ezptr<AnimationClip> Clip = StartIter->second;

		Length = Clip->Name.length();

		fwrite(&Length, sizeof(size_t), 1, File);
		fwrite(Clip->Name.c_str(), sizeof(char), Length, File);

		fwrite(&Clip->Option, sizeof(AnimationOption), 1, File);

		fwrite(&Clip->StartTime, sizeof(float), 1, File);
		fwrite(&Clip->EndTime, sizeof(float), 1, File);
		fwrite(&Clip->TimeLength, sizeof(float), 1, File);
		fwrite(&Clip->FrameTime, sizeof(float), 1, File);

		fwrite(&Clip->FrameMode, sizeof(int), 1, File);
		fwrite(&Clip->StartFrame, sizeof(int), 1, File);
		fwrite(&Clip->EndFrame, sizeof(int), 1, File);
		fwrite(&Clip->FrameLength, sizeof(int), 1, File);
		fwrite(&Clip->PlayTime, sizeof(float), 1, File);

		Count = Clip->KeyFrameVec.size();

		fwrite(&Count, sizeof(size_t), 1, File);

		for (size_t AA = 0; AA < Count; AA++)
		{
			fwrite(&Clip->KeyFrameVec[AA]->BoneIndex, sizeof(int), 1, File);

			size_t FrameCount = Clip->KeyFrameVec[AA]->KeyFrameVec.size();

			fwrite(&FrameCount, sizeof(size_t), 1, File);

			for (size_t BB = 0; BB < FrameCount; BB++)
			{
				fwrite(&Clip->KeyFrameVec[AA]->KeyFrameVec[BB]->Time, sizeof(double), 1, File);
				fwrite(&Clip->KeyFrameVec[AA]->KeyFrameVec[BB]->Pos, sizeof(Vec3), 1, File);
				fwrite(&Clip->KeyFrameVec[AA]->KeyFrameVec[BB]->Scale, sizeof(Vec3), 1, File);
				fwrite(&Clip->KeyFrameVec[AA]->KeyFrameVec[BB]->Rot, sizeof(Vec4), 1, File);
			}
		}
	}

	fclose(File);

	return true;
}

bool AIMAnimation::Load(const TCHAR * _FileName, const std::string & _Path)
{
	char FileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, _FileName, -1, FileName, lstrlen(_FileName), NULL, NULL);
#else
	strcpy_s(FileName, _FileName);
#endif

	return Load(FileName, _Path);
}

bool AIMAnimation::Load(const char * _FileName, const std::string & _Path)
{
	const char* Path = PathManager::FindPathMultiByte(_Path);

	std::string FullPath;

	if (Path != nullptr)
	{
		FullPath += Path;
	}

	FullPath += _FileName;

	return LoadFromFullPath(FullPath.c_str());
}

bool AIMAnimation::LoadFromFullPath(const TCHAR * _FullPath)
{
	char FileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, _FullPath, -1, FileName, lstrlen(_FullPath), NULL, NULL);
#else
	strcpy_s(FileName, _FullPath);
#endif

	return LoadFromFullPath(FileName);
}

bool AIMAnimation::LoadFromFullPath(const char * _FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, _FullPath, "rb");

	if (File == nullptr)
	{
		tassertmsg(true, "Failed to Load File");
		return false;
	}

	fread(&ChangeLimitTime, sizeof(float), 1, File);

	size_t Count = 0;
	size_t Length = 0;
	fread(&Count, sizeof(size_t), 1, File);

	char DefaultClipName[256] = {};
	fread(&Length, sizeof(size_t), 1, File);
	fread(DefaultClipName, sizeof(char), Length, File);

	char CurClipName[256] = {};
	fread(&Length, sizeof(size_t), 1, File);
	fread(CurClipName, sizeof(char), Length, File);

	AddClipNameList.clear();

	for (size_t AA = 0; AA < Count; AA++)
	{
		Ezptr<AnimationClip> Clip = new AnimationClip;

		char ClipName[256] = {};
		fread(&Length, sizeof(size_t), 1, File);
		fread(ClipName, sizeof(char), Length, File);

		ClipMap.insert(std::unordered_map<std::string, Ezptr<AnimationClip>>::value_type(ClipName, Clip));

		Clip->Name = ClipName;
		Clip->ChangeFrame = 0;

		AddClipNameList.push_back(ClipName);

		fread(&Clip->Option, sizeof(AnimationOption), 1, File);

		fread(&Clip->StartTime, sizeof(float), 1, File);
		fread(&Clip->EndTime, sizeof(float), 1, File);
		fread(&Clip->TimeLength, sizeof(float), 1, File);
		fread(&Clip->FrameTime, sizeof(float), 1, File);

		fread(&Clip->FrameMode, sizeof(int), 1, File);
		fread(&Clip->StartFrame, sizeof(int), 1, File);
		fread(&Clip->EndFrame, sizeof(int), 1, File);
		fread(&Clip->FrameLength, sizeof(int), 1, File);
		fread(&Clip->PlayTime, sizeof(float), 1, File);

		size_t FrameCount = 0;

		fread(&FrameCount, sizeof(size_t), 1, File);

		for (size_t BB = 0; BB < FrameCount; BB++)
		{
			Ezptr<BoneKeyFrame> TmpBoneKeyFrame = new BoneKeyFrame;

			Clip->KeyFrameVec.push_back(TmpBoneKeyFrame);

			fread(&TmpBoneKeyFrame->BoneIndex, sizeof(int), 1, File);

			size_t BoneFrameCount = 0;

			fread(&BoneFrameCount, sizeof(size_t), 1, File);

			for (size_t CC = 0; CC < BoneFrameCount; CC++)
			{
				Ezptr<KeyFrame> TmpKeyFrame = new KeyFrame;
				TmpBoneKeyFrame->KeyFrameVec.push_back(TmpKeyFrame);

				fread(&TmpKeyFrame->Time, sizeof(double), 1, File);
				fread(&TmpKeyFrame->Pos, sizeof(Vec3), 1, File);
				fread(&TmpKeyFrame->Scale, sizeof(Vec3), 1, File);
				fread(&TmpKeyFrame->Rot, sizeof(Vec4), 1, File);
			}
		}
	}

	CurClip = FindClip(CurClipName);
	DefaultClip = FindClip(DefaultClipName);

	fclose(File);
	
	return true;
}

bool AIMAnimation::SaveBone(const TCHAR * _FileName, const std::string & _Path)
{
	char FileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, _FileName, -1, FileName, lstrlen(_FileName), NULL, NULL);
#else
	strcpy_s(FileName, _FileName);
#endif

	return SaveBone(FileName, _Path);
}

bool AIMAnimation::SaveBone(const char * _FileName, const std::string & _Path)
{
	const char* Path = PathManager::FindPathMultiByte(_Path);

	std::string FullPath;

	if (Path != nullptr)
	{
		FullPath += Path;
	}

	FullPath += _FileName;

	return SaveBoneFromFullPath(FullPath.c_str());
}

bool AIMAnimation::SaveBoneFromFullPath(const TCHAR * _FullPath)
{
	char FileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, _FullPath, -1, FileName, lstrlen(_FullPath), NULL, NULL);
#else
	strcpy_s(FileName, _FullPath);
#endif

	return SaveBoneFromFullPath(FileName);
}

bool AIMAnimation::SaveBoneFromFullPath(const char * _FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, _FullPath, "wb");

	if (File == nullptr)
	{
		tassertmsg(true, "Failed To Save Bone");
		return false;
	}

	// 본 정보 저장 시작

	size_t Count = BoneVec.size();

	fwrite(&Count, sizeof(size_t), 1, File);

	size_t Length = 0;

	for (size_t i = 0; i < Count; i++)
	{
		Length = BoneVec[i]->Name.length();

		fwrite(&Length, sizeof(size_t), 1, File);
		fwrite(BoneVec[i]->Name.c_str(), sizeof(char), Length, File);

		fwrite(&BoneVec[i]->Depth, sizeof(int), 1, File);
		fwrite(&BoneVec[i]->ParendIndex, sizeof(int), 1, File);
		fwrite(&BoneVec[i]->OffsetMat->m, sizeof(DirectX::XMMATRIX), 1, File);
		fwrite(&BoneVec[i]->BoneMat->m, sizeof(DirectX::XMMATRIX), 1, File);
	}

	fclose(File);

	return true;
}

bool AIMAnimation::LoadBone(const TCHAR * _FileName, const std::string & _Path)
{
	char FileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, _FileName, -1, FileName, lstrlen(_FileName), NULL, NULL);
#else
	strcpy_s(FileName, _FileName);
#endif

	return LoadBone(FileName, _Path);
}

bool AIMAnimation::LoadBone(const char * _FileName, const std::string & _Path)
{
	const char* Path = PathManager::FindPathMultiByte(_Path);

	std::string FullPath;

	if (Path != nullptr)
	{
		FullPath += Path;
	}

	FullPath += _FileName;

	return LoadBoneFromFullPath(FullPath.c_str());
}

bool AIMAnimation::LoadBoneFromFullPath(const TCHAR * _FullPath)
{
	char FileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, _FullPath, -1, FileName, lstrlen(_FullPath), NULL, NULL);
#else
	strcpy_s(FileName, _FullPath);
#endif

	return LoadBoneFromFullPath(FileName);
}

bool AIMAnimation::LoadBoneFromFullPath(const char * _FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, _FullPath, "rb");

	if (File == nullptr)
	{
		tassertmsg(true, "Failed To Load Bone");
		return false;
	}

	size_t Count = 0;

	fread(&Count, sizeof(size_t), 1, File);

	size_t Length = 0;

	for (size_t i = 0; i < Count; i++)
	{
		Ezptr<Bone> TmpBone = new Bone;

		BoneVec.push_back(TmpBone);

		TmpBone->BoneMat = new Matrix;
		TmpBone->OffsetMat = new Matrix;

		char BoneName[256] = {};
		fread(&Length, sizeof(size_t), 1, File);
		fread(BoneName, sizeof(char), Length, File);

		TmpBone->Name = BoneName;

		fread(&TmpBone->Depth, sizeof(int), 1, File);
		fread(&TmpBone->ParendIndex, sizeof(int), 1, File);
		fread(&TmpBone->OffsetMat->m, sizeof(DirectX::XMMATRIX), 1, File);
		fread(&TmpBone->BoneMat->m, sizeof(DirectX::XMMATRIX), 1, File);
	}

	fclose(File);

	CreateBoneTexture();

	return true;
}

bool AIMAnimation::ModifyClip(const std::string & _Name, const std::string & _Change, AnimationOption _Option, int _StartFrame, int _EndFrame, float _PlayTime, const std::vector<Ezptr<BoneKeyFrame>>& _KeyFrameVec)
{
	Ezptr<AnimationClip> Clip = FindClip(_Name);

	if (Clip == nullptr)
	{
		return false;
	}

	ClipMap.erase(_Name);

	int Length = _EndFrame - _StartFrame;

	Clip->Option = _Option;
	Clip->Name = _Change;
	Clip->StartFrame = 0;
	Clip->EndFrame = Length;
	Clip->FrameLength = Length;
	Clip->StartTime = 0.0f;
	Clip->EndTime = _PlayTime;
	Clip->TimeLength = _PlayTime;
	Clip->PlayTime = _PlayTime;

	Clip->FrameTime = Clip->PlayTime / Length;

	ClipMap.insert(std::unordered_map<std::string, Ezptr<AnimationClip>>::value_type(_Change, Clip));

	Clip->KeyFrameVec.clear();

	for (size_t AA = 0; AA < _KeyFrameVec.size(); AA++)
	{
		Ezptr<BoneKeyFrame> BoneFrame = new BoneKeyFrame;

		Clip->KeyFrameVec.push_back(BoneFrame);

		Ezptr<BoneKeyFrame> ClipFrame = _KeyFrameVec[AA];
		BoneFrame->BoneIndex = ClipFrame->BoneIndex;

		if (false == ClipFrame->KeyFrameVec.empty())
		{
			for (size_t BB = _StartFrame; BB <= _EndFrame; BB++)
			{
				Ezptr<KeyFrame> Frame = new KeyFrame;

				Frame->Time = (BB - _StartFrame) * Clip->FrameTime;
				Frame->Pos = ClipFrame->KeyFrameVec[BB]->Pos;
				Frame->Scale = ClipFrame->KeyFrameVec[BB]->Scale;
				Frame->Rot = ClipFrame->KeyFrameVec[BB]->Rot;

				BoneFrame->KeyFrameVec.push_back(Frame);
			}
		}
	}

	return true;
}

bool AIMAnimation::DeleteClip(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<AnimationClip>>::iterator FindIter = ClipMap.find(_Name);

	if (FindIter == ClipMap.end())
	{
		return false;
	}

	if (_Name == DefaultClip->Name)
	{
		if (ClipMap.empty() == false)
		{
			std::unordered_map<std::string, Ezptr<AnimationClip>>::iterator FindIter2 = ClipMap.begin();

			while (FindIter2 != ClipMap.end() && FindIter2->first == _Name)
			{
				++FindIter2;
			}

			DefaultClip = FindClip(FindIter2->first);
		}
	}

	if (_Name == CurClip->Name)
	{
		CurClip = DefaultClip;
	}

	ClipMap.erase(FindIter);

	return true;
}

bool AIMAnimation::ReturnDefaultClip()
{
	ChangeClip(DefaultClip->Name);

	return true;
}

void AIMAnimation::LoadFbxAnimation(const char * _FullPath)
{
	FbxLoader Loader;

	Loader.LoadFbx(_FullPath);

	const std::vector<Ezptr<FbxAnimationClip>>* ClipVec = Loader.GetClipVec();

	std::vector<Ezptr<FbxAnimationClip>>::const_iterator StartIter = ClipVec->begin();
	std::vector<Ezptr<FbxAnimationClip>>::const_iterator EndIter = ClipVec->end();

	for (; StartIter != EndIter  ; ++StartIter)
	{
		AddClip(AO_LOOP, (*StartIter));
	}
}

void AIMAnimation::Start()
{
}

bool AIMAnimation::Init()
{
	return true;
}

int AIMAnimation::Input(float _Time)
{
	return 0;
}

int AIMAnimation::Update(float _Time)
{
	if (ClipMap.empty() == true)
	{
		return 0;
	}

	if (BoneMatrixVec.empty() == true)
	{
		BoneMatrixVec.resize(BoneVec.size());

		for (size_t i = 0; i < BoneMatrixVec.size(); i++)
		{
			BoneMatrixVec[i] = new Matrix;
		}
	}

	End = false;

	if (NextClip != nullptr)
	{
		ChangeTime += _Time;

		bool Change = false;

		if (ChangeTime >= ChangeLimitTime)
		{
			ChangeTime = ChangeLimitTime;
			Change = true;
		}

		float AnimationTime = AnimationGlobalTime + CurClip->StartTime;

		for (size_t i = 0; i < BoneVec.size(); i++)
		{
			if (CurClip->KeyFrameVec[i]->KeyFrameVec.empty() == true)
			{
				*(BoneMatrixVec[i]) = *(BoneVec[i]->BoneMat);
				continue;
			}

			int NextFrameIndex = NextClip->StartFrame;

			const Ezptr<KeyFrame> NextKey = NextClip->KeyFrameVec[i]->KeyFrameVec[NextFrameIndex];

			float Percent = ChangeTime / ChangeLimitTime;

			DirectX::XMVECTOR S = DirectX::XMVectorLerp(BlendInfoVec[i].Scale.Convert(), NextKey->Scale.Convert(), Percent);
			DirectX::XMVECTOR T = DirectX::XMVectorLerp(BlendInfoVec[i].Pos.Convert(), NextKey->Pos.Convert(), Percent);
			DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(BlendInfoVec[i].Rot.Convert(), NextKey->Rot.Convert(), Percent);

			DirectX::XMVECTOR Zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			Matrix BoneMat = DirectX::XMMatrixAffineTransformation(S, Zero, R, T);

			*(BoneVec[i])->BoneMat = BoneMat;

			BoneMat = *(BoneVec[i])->OffsetMat * BoneMat;

			*(BoneMatrixVec[i]) = BoneMat;
		}

		if (Change == true)
		{
			CurClip = NextClip;
			NextClip = nullptr;
			AnimationGlobalTime = 0.0f;
			ChangeTime = 0.0f;
		}
	}
	else
	{
		AnimationGlobalTime += _Time;
		ClipProgress = AnimationGlobalTime / CurClip->PlayTime;

		while (AnimationGlobalTime >= CurClip->PlayTime)
		{
			AnimationGlobalTime -= CurClip->PlayTime;
			End = true;
		}

		float AnimationTime = AnimationGlobalTime + CurClip->StartTime;

		int StartFrame = CurClip->StartFrame;
		int EndFrame = CurClip->EndFrame;

		int FrameIndex = (int)(AnimationTime / CurClip->FrameTime);

		if (End == true)
		{
			switch (CurClip->Option)
			{
			case AO_LOOP:
				FrameIndex = StartFrame;
				break;
			case AO_ONCE_DESTROY:
				Object->SetAlive(false);
				break;
			}
		}
		else
		{
			int NextFrameIndex = FrameIndex + 1;

			CurClip->ChangeFrame = FrameIndex;

			if (NextFrameIndex > EndFrame)
			{
				NextFrameIndex = StartFrame;
			}

			for (size_t i = 0; i < BoneVec.size(); i++)
			{
				if (CurClip->KeyFrameVec[i]->KeyFrameVec.empty() == true)
				{
					*(BoneMatrixVec[i]) = *(BoneVec[i])->BoneMat;
					continue;
				}

				const Ezptr<KeyFrame> CurKey = CurClip->KeyFrameVec[i]->KeyFrameVec[FrameIndex];
				const Ezptr<KeyFrame> NextKey = CurClip->KeyFrameVec[i]->KeyFrameVec[NextFrameIndex];

				double FrameTime = CurKey->Time;

				float Percent = (float)((AnimationTime - FrameTime) / CurClip->FrameTime);

				DirectX::XMVECTOR S = DirectX::XMVectorLerp(CurKey->Scale.Convert(), NextKey->Scale.Convert(), Percent);
				DirectX::XMVECTOR T = DirectX::XMVectorLerp(CurKey->Pos.Convert(), NextKey->Pos.Convert(), Percent);
				DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(CurKey->Rot.Convert(), NextKey->Rot.Convert(), Percent);

				BlendInfoVec[i].Pos = T;
				BlendInfoVec[i].Scale = S;
				BlendInfoVec[i].Rot = R;

				DirectX::XMVECTOR Zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

				Matrix BoneMat = DirectX::XMMatrixAffineTransformation(S, Zero, R, T);

				*(BoneVec[i])->BoneMat = BoneMat;

				BoneMat = *(BoneVec[i])->OffsetMat * BoneMat;

				*(BoneMatrixVec[i]) = BoneMat;
				
			}
		}
	}

	if (End == false)
	{
		D3D11_MAPPED_SUBRESOURCE Map = {};

		GetAIMContext->Map(BoneTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

		Matrix* Mat = (Matrix*)Map.pData;

		for (size_t i = 0; i < BoneMatrixVec.size(); i++)
		{
			Mat[i] = *(BoneMatrixVec[i]);
		}

		GetAIMContext->Unmap(BoneTex, 0);
	}

	return 0;
}

int AIMAnimation::LateUpdate(float _Time)
{
	return 0;
}

int AIMAnimation::Collision(float _Time)
{
	return 0;
}

int AIMAnimation::PrevRender(float _Time)
{
	GetAIMContext->VSSetShaderResources(3, 1, &BoneSRV);

	return 0;
}

int AIMAnimation::Render(float _Time)
{
	return 0;
}

AIMAnimation * AIMAnimation::Clone() const
{
	return new AIMAnimation(*this);
}

