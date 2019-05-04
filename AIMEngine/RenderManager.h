#pragma once

#include "AIMObject.h"
#include "RefCounter.h"
#include "RenderTarget.h"

// 깨달은 점. 이 포인터로는 더블포인터는 힘들 것 같다.
class Engine_DLL RenderList
{
public:
	AIMObject** ObjList = nullptr;
	int Size = 0;
	int Capacity = 100;

	RenderList()
	{
		ObjList = new AIMObject*[Capacity];
	}

	~RenderList()
	{
		if (ObjList != nullptr)
		{
			delete[] ObjList;
			ObjList = nullptr;
		}
	}

	// 필요없을듯
	// ~RenderList()
};

class Engine_DLL MultiRenderTarget : public RefCounter
{
public:
	std::vector<Ezptr<RenderTarget>> RenderTargetVec;
	std::vector<ID3D11RenderTargetView*> OldTargetVec;
	ID3D11DepthStencilView* DepthView = nullptr;
	ID3D11DepthStencilView* OldDepthView = nullptr;
};

class AIMLight;
class AIMSampler;
class RenderState;
class RenderManager
{
private:
	static std::unordered_map<std::string, Ezptr<RenderState>> RenderStateMap;
	static std::unordered_map<std::string, Ezptr<RenderTarget>> RenderTargetMap;
	static std::unordered_map<std::string, Ezptr<MultiRenderTarget>> MultiRenderTargetMap;
	static RenderList LightList;
	static RenderList RenderGroupList[RG_END];
	static RenderMode RM;
	static RenderConstBuffer RenderCBuffer;

	static Ezptr<AIMSampler> LinearSampler;
	static Ezptr<AIMSampler> PointSampler;
	static Ezptr<RenderState> DepthDisable;

public:
	static Ezptr<AIMLight> GetFirstLight();
public:
	static bool Init();
	static void Release();

	static void AddRenderObject(Ezptr<AIMObject> _Obj);

public:
	static void Render(float _Time);

public:
	static bool CreateRasterizerState(const std::string& _Name, D3D11_FILL_MODE _Fill = D3D11_FILL_SOLID, D3D11_CULL_MODE _Cull = D3D11_CULL_BACK);
	static bool CreateDepthState(const std::string& _Name, BOOL _Enable = TRUE, D3D11_DEPTH_WRITE_MASK _WriteMask = D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_FUNC _DepthFunc = D3D11_COMPARISON_LESS);

	static bool CreateRenderTarget(const std::string& _Name, UINT _Width, UINT _Height, DXGI_FORMAT _Format, float _ClearColor[4], DXGI_FORMAT _DepthFormat = DXGI_FORMAT_UNKNOWN);
	static bool OnDebugRenderTarget(const std::string& _Name, const Vec3& _Pos, const Vec3& _Scale);

	static bool AddMultiRenderTarget(const std::string& _Name, const std::string& _Target);
	static bool AddDepth(const std::string& _Name, const std::string& _Target);
	static void ClearMultiRenderTarget(const std::string& _Name);
	static void SetMultiRenderTarget(const std::string& _Name);
	static void ResetMultiRenderTarget(const std::string& _Name);

public:
	static Ezptr<RenderState> FindRenderState(const std::string& _Name);
	static Ezptr<RenderTarget> FindRenderTarget(const std::string& _Name);
	static Ezptr<MultiRenderTarget> FindMultiRenderTarget(const std::string& _Name);

private:
	static void RenderForward(float _Time);
	static void RenderDeferred(float _Time);
	static void RenderGBuffer(float _Time);
private:
	RenderManager() {}
	virtual ~RenderManager() = 0 {}
};

