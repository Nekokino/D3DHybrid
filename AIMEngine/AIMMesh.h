#pragma once

#include "RefCounter.h"
#include "AIMMaterial.h"
#include "AIMAnimation.h"
typedef struct Engine_DLL _tagVertexBuffer
{
public:
	ID3D11Buffer* Buffer;
	void* Data;
	int Size;
	int Count;
	D3D11_USAGE Usage;
	D3D11_PRIMITIVE_TOPOLOGY Primitive;
}VertexBuffer, *PVertexBuffer;

typedef struct Engine_DLL _tagIndexBuffer
{
public:
	ID3D11Buffer* Buffer;
	void* Data;
	int Size;
	int Count;
	D3D11_USAGE Usage;
	DXGI_FORMAT Format;
}IndexBuffer, *PIndexBuffer;

// 메쉬 컨테이너 하나에는 한개의 버텍스버퍼와 여러개의 인덱스 버퍼로 이루어지게 했다.
typedef struct Engine_DLL _tagMeshContainer
{
public:
	VertexBuffer VB;
	std::vector<IndexBuffer*> IBVec;
}MeshContainer, *PMeshContainer;

class FbxLoader;
class Engine_DLL AIMMesh : public RefCounter
{
public:
	friend class ResourceManager;

private:
	std::vector<MeshContainer*> MeshContainerVec;
	std::string ShaderName;
	std::string InputLayoutName;
	Vec3 View;
	Vec3 Min;
	Vec3 Max;
	Vec3 Length;
	Vec3 Center;
	float Radius = 10.0f;
	Ezptr<AIMMaterial> Material = nullptr;
	Ezptr<AIMAnimation> Animation = nullptr;
public:
	std::string GetShaderName() const
	{
		return ShaderName;
	}
	std::string GetInputLayoutName() const
	{
		return InputLayoutName;
	}
	size_t GetContainerCount() const
	{
		return MeshContainerVec.size();
	}
	size_t GetSubsetCount(int _Container = 0) const
	{
		return MeshContainerVec[_Container]->IBVec.size();
	}
	Vec3 GetView() const
	{
		return View;
	}
	Vec3 GetMin() const
	{
		return Min;
	}
	Vec3 GetMax() const
	{
		return Max;
	}
	Vec3 GetCenter() const
	{
		return Center;
	}

	float GetRadius() const
	{
		return Radius;
	}

	void SetView(const Vec3& _Vec)
	{
		View = _Vec;
	}

public:
	bool CreateMesh(const std::string& _Name, const std::string& _ShaderName, const std::string& _InputLayoutName, int _VtxSize, int _VtxCount, D3D11_USAGE _VtxUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void* _Vtx,
		int _IdxSize = 0, int _IdxCount = 0, D3D11_USAGE _IdxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT _Format = DXGI_FORMAT_UNKNOWN, void* _Idx = nullptr);
	bool LoadMesh(const std::string& _Name, const TCHAR* _FileName, const Vec3& _View = Vec3::Axis[AXIS_Z], const std::string& _Path = "Mesh");
	bool LoadMesh(const std::string& _Name, const char* _FileName, const Vec3& _View = Vec3::Axis[AXIS_Z], const std::string& _Path = "Mesh");
	bool LoadMeshFromFullPath(const std::string& _Name, const TCHAR* _FullPath, const Vec3& _View = Vec3::Axis[AXIS_Z]);
	bool LoadMeshFromFullPath(const std::string& _Name, const char* _FullPath, const Vec3& _View = Vec3::Axis[AXIS_Z]);

	// 모든 컨테이너의 모든 서브셋을 다 그리는 렌더
	void Render();
	// 특정 컨테이너의 특정 서브셋을 그리는 렌더
	void Render(size_t _Container, size_t _SubSet = 0);
	void RenderInstancing(unsigned int _Container, unsigned int _Subset, InstancingBuffer* _InstancingBuffer, int _InstancingCount);

private:
	bool CreateVertexBuffer(int _Size, int _Count, D3D11_USAGE _Usage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void* _Data);
	bool CreateIndexBuffer(int _Size, int _Count, D3D11_USAGE _Usage, DXGI_FORMAT _Format, void* _Data);

private:
	bool ConvertFbx(FbxLoader* _Loader, const char* _FullPath);

public:
	bool Save(const char* _FileName, const std::string& _Path = "Mesh");
	bool SaveFullPath(const char* _FullPath);
	bool Load(const char* _FileName, const std::string& _Path = "Mesh");
	bool LoadFullPath(const char* _FullPath);

	Ezptr<AIMMaterial> CloneMaterial();
	Ezptr<AIMAnimation> CloneAnimation();

private:
	AIMMesh();
	~AIMMesh();
};