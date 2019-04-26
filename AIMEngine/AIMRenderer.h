#pragma once
#include "AIMComponent.h"
#include "AIMMesh.h"
#include "AIMShader.h"

class Engine_DLL AIMRenderer : public AIMComponent
{
	friend class AIMObject;

private:
	Ezptr<AIMShader> Shader;
	Ezptr<AIMMesh> Mesh;
	ID3D11InputLayout* InputLayout = nullptr;

public:
	void SetMesh(const std::string& _Key);
	void SetShader(const std::string& _Key);
	void SetInputLayout(const std::string& _Key);

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMRenderer* Clone() const;

private:
	AIMRenderer();
	AIMRenderer(const AIMRenderer& _Other);
	~AIMRenderer();
};

