#pragma once
#include "AIMComponent.h"

class Engine_DLL AIMParticle : public AIMComponent
{
	friend class AIMObject;

private:
	ParticleConstBuffer CBuffer;

public:
	bool LoadTexture(const std::string& _Name, const std::vector<TCHAR*>& _FileNameVec, const std::string& _Path);
	bool LoadTexture(const std::string& _Name, const std::vector<TCHAR*>& _FullPathVec);

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

