#pragma once

#include "RefCounter.h"
#include "AIMMesh.h"

class AIMTexture;
class AIMSampler;
class Engine_DLL ResourceManager
{
private:
	static std::unordered_map<std::string, Ezptr<AIMMesh>> MeshMap;
	static std::unordered_map<std::string, Ezptr<AIMTexture>> TextureMap;
	static std::unordered_map<std::string, Ezptr<AIMSampler>> SamplerMap;

	static std::vector<Vertex3D> SphereVtxVec;
	static std::vector<UINT> SphereIdxVec;

public:
	static bool Init();
	static void Release();

	static bool CreateMesh(const std::string& _Name, const std::string& _ShaderName, const std::string& _InputLayoutName, const Vec3& _View, int _VtxSize, int _VtxCount, D3D11_USAGE _VtxUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive,
		void* _Vtx, int _IdxSize = 0, int _IdxCount = 0, D3D11_USAGE _IdxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT _Format = DXGI_FORMAT_UNKNOWN, void* _Idx = nullptr);
	static bool CreateSpherePos(const std::string& _Name, const std::string& _Shader, const std::string& _InputLayout);

	static Ezptr<AIMMesh> FindMesh(const std::string& _Name);

	static bool LoadTexture(const std::string& _Name, const TCHAR* _FileName, const std::string& _Path);
	static Ezptr<AIMTexture> FindTexture(const std::string& _Name);

	static bool CreateSampler(const std::string& _Name, D3D11_FILTER _Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE _AddU = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE _AddV = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE _AddW = D3D11_TEXTURE_ADDRESS_WRAP);

	static Ezptr<AIMSampler> FindSampler(const std::string& _Name);

private:
	static bool CreateSphere(float _Rad, unsigned int _SubDiv);
	static void SubDivide();
	static float AngleFromXY(float _X, float _Y);
private:
	ResourceManager() {}
	virtual ~ResourceManager() = 0 {}
};

