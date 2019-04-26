#pragma once

#include "RefCounter.h"

class AIMMesh;
class Engine_DLL ResourceManager
{
private:
	static std::unordered_map<std::string, Ezptr<AIMMesh>> MeshMap;

public:
	static bool Init();
	static void Release();

	static bool CreateMesh(const std::string& _Name, const std::string& _ShaderName, const std::string& _InputLayoutName, int _VtxSize, int _VtxCount, D3D11_USAGE _VtxUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive,
		void* _Vtx, int _IdxSize = 0, int _IdxCount = 0, D3D11_USAGE _IdxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT _Format = DXGI_FORMAT_UNKNOWN, void* _Idx = nullptr);

	static Ezptr<AIMMesh> FindMesh(const std::string& _Name);
private:
	ResourceManager() {}
	virtual ~ResourceManager() = 0 {}
};

