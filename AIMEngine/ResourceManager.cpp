#include "ResourceManager.h"
#include "AIMMesh.h"

std::unordered_map<std::string, Ezptr<AIMMesh>> ResourceManager::MeshMap;


bool ResourceManager::Init()
{
	ColorVertex TriColor[3]
	{
		ColorVertex(Vec3(0.0f, 0.5f, 0.0f), Vec4::Green),
		ColorVertex(Vec3(0.5f, -0.5f, 0.0f), Vec4::Red),
		ColorVertex(Vec3(-0.5f, -0.5f, 0.0f), Vec4::Blue)
	};

	int Idx[3] = { 0, 1, 2 };

	CreateMesh("ColorTriangle", sizeof(ColorVertex), 3, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, TriColor, 4, 3, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, Idx);

	return true;
}

void ResourceManager::Release()
{
	MeshMap.clear();
}

bool ResourceManager::CreateMesh(const std::string & _Name, int _VtxSize, int _VtxCount, D3D11_USAGE _VtxUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void * _Vtx, int _IdxSize, int _IdxCount, D3D11_USAGE _IdxUsage, DXGI_FORMAT _Format, void * _Idx)
{
	Ezptr<AIMMesh> Mesh = FindMesh(_Name);

	if (nullptr != Mesh)
	{
		tassertmsg(true, "Can't Create Mesh (Name Overlapped)");
		return false;
	}

	Mesh = new AIMMesh;

	if (false == Mesh->CreateMesh(_Name, _VtxSize, _VtxCount, _VtxUsage, _Primitive, _Vtx, _IdxSize, _IdxCount, _IdxUsage, _Format, _Idx))
	{
		delete Mesh;
		return false;
	}

	MeshMap.insert(std::unordered_map<std::string, Ezptr<AIMMesh>>::value_type(_Name, Mesh));

	return true;
}

Ezptr<AIMMesh> ResourceManager::FindMesh(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<AIMMesh>>::iterator iter = MeshMap.find(_Name);

	if (iter == MeshMap.end())
	{
		return nullptr;
	}

	return iter->second;
}