#pragma once

#include "AIMComponent.h"
#include "AIMSampler.h"
#include "AIMTexture.h"

class Engine_DLL TextureSet : public RefCounter
{
public:
	Ezptr<AIMTexture> Texture;
	int TexReg = 0;
};

class Engine_DLL MaterialSubset : public RefCounter
{
public:
	Material Mtrl;
	std::unordered_map<std::string, Ezptr<TextureSet>> TextureSetMap;

	~MaterialSubset()
	{
		TextureSetMap.clear();
	}
};

class Engine_DLL MaterialContainer : public RefCounter
{
public:
	std::vector<Ezptr<MaterialSubset>> SubsetVec;
};

class Engine_DLL AIMMaterial : public AIMComponent
{
	friend class AIMObject;
	friend class AIMMesh;
private:
	std::vector<Ezptr<MaterialContainer>> ContainerVec;

public:
	void SetColor(const Vec4& _Dif, const Vec4& _Amb, const Vec4& _Spc, const Vec4& _Emv, float _SpcPower, UINT _Container = 0, UINT _Subset = 0);

	void SetEmissiveColor(float _Emv);
	void SetEmissiveColor(int _Container, int _Subset, float _Emv);

	void AddTextureSet(int _Container, int _Subset, int _TexReg, const std::string& _TexName, const TCHAR* _FileName = nullptr, const std::string& _Path = "Texture");
	void AddTextureSet(int _Container, int _Subset, int _TexReg, Ezptr<AIMTexture> _Texture);
	void AddTextureSetFromFullPath(int _Container, int _Subset, int _TexReg, const std::string& _TexName, const TCHAR* _FullPath = nullptr);
	void AddTextureSetArray(int _Container, int _Subset, int _TexReg, const std::string& _TexName, const std::vector<TCHAR*>* _FileNameVec = nullptr, const std::string& _Path = "Texture");
	void AddTextureSetArrayFromFullPath(int _Container, int _Subset, int _TexReg, const std::string& _TexName, const std::vector<TCHAR*>* _FullPathVec = nullptr);
	void DeleteTextureSet(int _Container, int _Subset, const std::string& _Name);
	void DeleteTextureSet(int _Container, int _Subset);
	void ChangeTextureSet(int _Container, int _Subset, int _TexReg, const std::string& _Name, const std::string& _ChangeName, const TCHAR* _FileName = nullptr, const std::string& _Path = "Texture");
	void ChangeTextureSet(int _Container, int _Subset, int _TexReg, const std::string& _Name, Ezptr<AIMTexture> _Texture);
	void ChangeTextureSetFromFullPath(int _Container, int _Subset, int _TexReg, const std::string& _Name, const std::string& _ChangeName, const TCHAR* _FullPath = nullptr);
	void ChangeTextureSetArray(int _Container, int _Subset, int _TexReg, const std::string& _Name, const std::string& _ChangeName, const std::vector<TCHAR*>* _FileNameVec = nullptr, const std::string& _PathName = "Texture");
	void ChangeTextureSetArrayFromFullPath(int _Container, int _Subset, int _TexReg, const std::string& _Name, const std::string& _ChangeName, const std::vector<TCHAR*>* _FullPathVec = nullptr);

	void BumpTextureEnable(int _Container, int _Subset);
	void SpecularTextureEnable(int _Container, int _Subset);
	void Skinning(int _Container, int _Subset);

private:
	Ezptr<TextureSet> FindTextureSet(int _Container, int _Subset, const std::string& _Name);

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMMaterial* Clone() const;

public:
	void SetShader(int _Container, int _Subset);

	virtual void Save(FILE* _File);
	virtual void Load(FILE* _File);
	virtual void SaveFromPath(const char* _FileName, const std::string& _Path = "Data");
	virtual void LoadFromPath(const char* _FileName, const std::string& _Path = "Data");

private:
	void SaveTextureSet(FILE* _File, Ezptr<TextureSet> _Texture);
	void LoadTextureSet(FILE* _File, Ezptr<TextureSet>* _Texture);

protected:
	AIMMaterial();
	AIMMaterial(const AIMMaterial& _Other);
	~AIMMaterial();
};

