#pragma once
#include "AIMComponent.h"
#include "AIMMesh.h"
#include "AIMShader.h"
#include "RenderState.h"

class AIMMaterial;
class Engine_DLL AIMRenderer : public AIMComponent
{
	friend class AIMObject;

private:
	Ezptr<AIMShader> Shader;
	Ezptr<AIMMesh> Mesh;
	ID3D11InputLayout* InputLayout = nullptr;
	Ezptr<AIMMaterial> Material;
	Ezptr<RenderState> RenderStateList[RS_END];

public:
	Ezptr<AIMMesh> GetMesh() const
	{
		return Mesh;
	}

public:
	void SetMesh(const std::string& _Key);
	void SetMesh(const std::string& _Key, const TCHAR* _FileName, const Vec3& _View = Vec3::Axis[AXIS_Z], const std::string& _Path = "Mesh");
	void SetMeshFromFullPath(const std::string& _Key, const TCHAR* _FileName, const Vec3& _View = Vec3::Axis[AXIS_Z]);
	void SetShader(const std::string& _Key);
	void SetInputLayout(const std::string& _Key);
	void SetRenderState(const std::string& _Key);

public:
	void RenderInstancing(InstancingBuffer* _Buffer, Ezptr<AIMShader> _Shader, ID3D11InputLayout* _Layout, int _InstancingCount, float _Time);

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

