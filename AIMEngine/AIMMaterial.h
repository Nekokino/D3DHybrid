#pragma once

#include "AIMComponent.h"
#include "AIMSampler.h"
#include "AIMTexture.h"

class Engine_DLL TextureSet : public RefCounter
{
public:
	Ezptr<AIMTexture> Texture = nullptr;
	Ezptr<AIMSampler> Sampler = nullptr;
	int TexReg = 0;
	int SmpReg = 0;
};

class Engine_DLL MaterialSubset : public RefCounter
{
public:
	Material Mtrl;
	Ezptr<TextureSet> Diffuse = nullptr;
};

class Engine_DLL MaterialContainer : public RefCounter
{
public:
	std::vector<Ezptr<MaterialSubset>> SubsetVec;
};

class Engine_DLL AIMMaterial : public AIMComponent
{
	friend class AIMObject;
private:
	std::vector<Ezptr<MaterialContainer>> ContainerVec;

public:
	void SetColor(const Vec4& _Dif, const Vec4& _Amb, const Vec4& _Spc, const Vec4& _Emv, float _SpcPower, UINT _Container = 0, UINT _Subset = 0);
	void SetDiffuseTextureSet(int _Container, int _Subset, const std::string& _SmpName, int _SmpReg, int _TexReg, const std::string& _TexName, const TCHAR* _FileName = nullptr, const std::string& _Path = "Texture");

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

protected:
	AIMMaterial();
	AIMMaterial(const AIMMaterial& _Other);
	~AIMMaterial();
};

