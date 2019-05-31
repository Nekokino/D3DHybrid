#pragma once
#include "AIMComponent.h"

class AIMMaterial;
class Engine_DLL AIMParticle : public AIMComponent
{
	friend class AIMObject;

private:
	ParticleConstBuffer CBuffer;
	Ezptr<AIMMaterial> Material;
	std::string TextureName;

public:
	bool LoadTextureSet(const std::string& _Name, const TCHAR* _FileName = nullptr, const std::string& _Path = "Texture");
	bool LoadTextureSetFromFullPath(const std::string& _Name, const TCHAR* _FullPath = nullptr);
	bool LoadTextureSet(const std::string& _Name, const std::vector<TCHAR*>& _FileName, const std::string& _Path = "Texture");
	bool LoadTextureSetFromFullPath(const std::string& _Name, const std::vector<TCHAR*>& _FullPath);

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMParticle* Clone() const;

protected:
	AIMParticle();
	AIMParticle(const AIMParticle& _Other);
	~AIMParticle();
};

