#pragma once

#include "AIMComponent.h"

class Engine_DLL SplattingName : public RefCounter
{
public:
	std::vector<TCHAR*> DifName;
	std::vector<TCHAR*> NrmName;
	std::vector<TCHAR*> SpcName;
	std::vector<TCHAR*> AlpName;
	std::vector<std::string> PathName;

	
	~SplattingName()
	{
		for (size_t i = 0; i < DifName.size(); i++)
		{
			if (DifName[i] != nullptr)
			{
				delete[] DifName[i];
				DifName[i] = nullptr;
			}
		}

		for (size_t i = 0; i < NrmName.size(); i++)
		{
			if (NrmName[i] != nullptr)
			{
				delete[] NrmName[i];
				NrmName[i] = nullptr;
			}
		}

		for (size_t i = 0; i < SpcName.size(); i++)
		{
			if (SpcName[i] != nullptr)
			{
				delete[] SpcName[i];
				SpcName[i] = nullptr;
			}
		}

		for (size_t i = 0; i < AlpName.size(); i++)
		{
			if (AlpName[i] != nullptr)
			{
				delete[] AlpName[i];
				AlpName[i] = nullptr;
			}
		}
	}
};


class AIMTexture;
class AIMMaterial;
class Engine_DLL AIMLandScape : public AIMComponent
{
	friend class AIMObject;

private:
	std::vector<Vertex3D> VtxVec;
	std::vector<Vec3> FaceNormal;
	std::vector<int> IdxVec;
	int NumX = 0;
	int NumZ = 0;
	LandScapeConstBuffer CBuffer;
	Ezptr<AIMMaterial> Material;
	SplattingName SplatName;
	Ezptr<AIMTexture> SpDifTex;
	Ezptr<AIMTexture> SpNrmTex;
	Ezptr<AIMTexture> SpSpcTex;
	Ezptr<AIMTexture> SpAlpTex;


public:
	bool CreateLandScape(const std::string& _Name, const char* _FileName, const std::string& _PathName = "Texture");
	void SetDetailLevel(float _LevelX, float _LevelZ);
	void ChangeBaseTexture(const TCHAR* _DifName, const TCHAR* _NrmName, const TCHAR* _SpcName, const std::string& _Path = "Texture");
	void AddSplattingTexture(const TCHAR* _DifName, const TCHAR* _NrmName, const TCHAR* _SpcName, const TCHAR* _AlpName, const std::string& _Path = "Texture");
	void CreateSplattingTexture();

private:
	void ComputeNormal();
	void ComputeTangent();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMLandScape* Clone() const;

protected:
	AIMLandScape();
	AIMLandScape(const AIMLandScape& _Other);
	~AIMLandScape();
};

