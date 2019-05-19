#pragma once

#include "RefCounter.h"
#include "AIMMaterial.h"
#include "AIMAnimation.h"

typedef struct Engine_DLL _tagVertexBuffer
{
	ID3D11Buffer* Buffer;
	void* Data;
	int Size;
	int Count;
	D3D11_USAGE Usage;
	D3D11_PRIMITIVE_TOPOLOGY Primitive;
}VertexBuffer, *PVertexBuffer;

typedef struct Engine_DLL _tagIndexBuffer
{
	ID3D11Buffer* Buffer;
	void* Data;
	int Size;
	int Count;
	D3D11_USAGE Usage;
	DXGI_FORMAT Format;
}IndexBuffer, *PIndexBuffer;

// �޽� �����̳� �ϳ����� �Ѱ��� ���ؽ����ۿ� �������� �ε��� ���۷� �̷������ �ߴ�.
typedef struct Engine_DLL _tagMeshContainer
{
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
	float Radius = 0.0f;
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

	// ��� �����̳��� ��� ������� �� �׸��� ����
	void Render();
	// Ư�� �����̳��� Ư�� ������� �׸��� ����
	void Render(size_t _Container, size_t _SubSet = 0);

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