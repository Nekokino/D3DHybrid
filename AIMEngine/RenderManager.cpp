#include "RenderManager.h"
#include "ShaderManager.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "AIMLight.h"
#include "AIMDevice.h"
#include "ResourceManager.h"

std::unordered_map<std::string, Ezptr<RenderState>> RenderManager::RenderStateMap;
std::unordered_map<std::string, Ezptr<RenderTarget>> RenderManager::RenderTargetMap;
std::unordered_map<std::string, Ezptr<MultiRenderTarget>> RenderManager::MultiRenderTargetMap;
RenderList RenderManager::LightList;
RenderList RenderManager::RenderGroupList[RG_END];
RenderMode RenderManager::RM = RM_DEFERRED;
RenderConstBuffer RenderManager::RenderCBuffer;
Ezptr<AIMSampler> RenderManager::LinearSampler;
Ezptr<AIMSampler> RenderManager::PointSampler;
Ezptr<RenderState> RenderManager::DepthDisable;

Ezptr<AIMLight> RenderManager::GetFirstLight()
{
	if (LightList.Size == 0)
	{
		return nullptr;
	}

	return LightList.ObjList[0]->FindComponent<AIMLight>(CT_LIGHT);
}

bool RenderManager::Init()
{
	RenderCBuffer.RenderMode = RM;

	if (false == ShaderManager::Init())
	{
		tassertmsg(true, "ShaderManger Init Failed");
		return false;
	}

	CreateRasterizerState("CullNone", D3D11_FILL_SOLID, D3D11_CULL_NONE);

	CreateDepthState("LessEqual", TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);

	CreateDepthState("DepthDisable", FALSE);

	DepthDisable = FindRenderState("DepthDisable");

	float ClearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	if (false == CreateRenderTarget("Albedo", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("Albedo", Vec3(0.0f, 0.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("Normal", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("Normal", Vec3(0.0f, 100.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("Depth", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("Depth", Vec3(0.0f, 200.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("MaterialDif", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("MaterialDif", Vec3(0.0f, 300.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("MaterialAmb", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("MaterialAmb", Vec3(0.0f, 400.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("MaterialSpc", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("MaterialSpc", Vec3(0.0f, 500.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("MaterialEmv", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("MaterialEmv", Vec3(0.0f, 600.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	AddMultiRenderTarget("GBuffer", "Albedo");
	AddMultiRenderTarget("GBuffer", "Normal");
	AddMultiRenderTarget("GBuffer", "Depth");
	AddMultiRenderTarget("GBuffer", "MaterialDif");
	AddMultiRenderTarget("GBuffer", "MaterialAmb");
	AddMultiRenderTarget("GBuffer", "MaterialSpc");
	AddMultiRenderTarget("GBuffer", "MaterialEmv");

	LinearSampler = ResourceManager::FindSampler("LinearSampler");
	PointSampler = ResourceManager::FindSampler("PointSampler");

	return true;
}

void RenderManager::Release()
{
	LinearSampler = nullptr;
	PointSampler = nullptr;
	DepthDisable = nullptr;
	MultiRenderTargetMap.clear();
	RenderStateMap.clear();
	RenderTargetMap.clear();
	ShaderManager::Release();
}

void RenderManager::AddRenderObject(Ezptr<AIMObject> _Obj)
{
	RenderGroup Tmp = RG_END;

	if (_Obj->CheckComponent(CT_LIGHT) == true)
	{
		if (LightList.Size == LightList.Capacity)
		{
			LightList.Capacity *= 2;

			AIMObject** List = new AIMObject*[LightList.Capacity];

			memcpy(List, LightList.ObjList, sizeof(AIMObject*) * LightList.Size);

			if (LightList.ObjList != nullptr)
			{
				delete[] LightList.ObjList;
				LightList.ObjList = nullptr;
			}
			LightList.ObjList = List;
		}

		LightList.ObjList[LightList.Size] = _Obj;
		++LightList.Size;
	}


	if (_Obj->CheckComponent(CT_RENDERER) == false)
	{
		return; 
	}
	else if (_Obj->CheckComponent(CT_LANDSCAPE) == true)
	{
		Tmp = RG_LANDSCAPE;
	}
	else if (_Obj->CheckComponent(CT_UI) == true)
	{
		Tmp = RG_UI;
	}
	else
		Tmp = RG_DEFAULT;

	if (RenderGroupList[Tmp].Size == RenderGroupList[Tmp].Capacity)
	{
		RenderGroupList[Tmp].Capacity *= 2;

		AIMObject** List = new AIMObject*[RenderGroupList[Tmp].Capacity];

		memcpy(List, RenderGroupList[Tmp].ObjList, sizeof(AIMObject*) * RenderGroupList[Tmp].Size);

		if (RenderGroupList[Tmp].ObjList != nullptr)
		{
			delete[] RenderGroupList[Tmp].ObjList;
			RenderGroupList[Tmp].ObjList = nullptr;
		}

		RenderGroupList[Tmp].ObjList = List;
	}

	RenderGroupList[Tmp].ObjList[RenderGroupList[Tmp].Size] = _Obj;
	++RenderGroupList[Tmp].Size;
}

void RenderManager::Render(float _Time)
{
	ShaderManager::UpdateConstBuffer("Render", &RenderCBuffer);

	switch (RM)
	{
	case RM_FORWARD:
		RenderForward(_Time);
		break;
	case RM_DEFERRED:
		RenderDeferred(_Time);
		break;
	}

	LinearSampler->PSSetSampler(0);

	DepthDisable->SetState();

	std::unordered_map<std::string, Ezptr<RenderTarget>>::iterator StartIter = RenderTargetMap.begin();
	std::unordered_map<std::string, Ezptr<RenderTarget>>::iterator EndIter = RenderTargetMap.end();

	for (; StartIter != EndIter  ; ++StartIter)
	{
		StartIter->second->Render();
	}

	DepthDisable->ResetState();

	LightList.Size = 0;
}

bool RenderManager::CreateRasterizerState(const std::string & _Name, D3D11_FILL_MODE _Fill, D3D11_CULL_MODE _Cull)
{
	Ezptr<RasterizerState> State = FindRenderState(_Name);

	if (State != nullptr)
	{
		tassertmsg(true, "Overlapped State Name");
		return false;
	}

	State = new RasterizerState;

	if (State->CreateState(_Name, _Fill, _Cull) == false)
	{
		tassertmsg(true, "RasterizerState Failed");
		return false;
	}

	RenderStateMap.insert(std::unordered_map<std::string, Ezptr<RenderState>>::value_type(_Name, State));

	return true;
}

bool RenderManager::CreateDepthState(const std::string & _Name, BOOL _Enable, D3D11_DEPTH_WRITE_MASK _WriteMask, D3D11_COMPARISON_FUNC _DepthFunc)
{
	Ezptr<DepthStencilState> State = FindRenderState(_Name);

	if (State != nullptr)
	{
		tassertmsg(true, "Overlapped State Name");
		return false;
	}

	State = new DepthStencilState;

	if (State->CreateState(_Name, _Enable, _WriteMask, _DepthFunc) == false)
	{
		tassertmsg(true, "DepthState Failed");
		return false;
	}

	RenderStateMap.insert(std::unordered_map<std::string, Ezptr<RenderState>>::value_type(_Name, State));

	return true;
}

bool RenderManager::CreateRenderTarget(const std::string & _Name, UINT _Width, UINT _Height, DXGI_FORMAT _Format, float _ClearColor[4], DXGI_FORMAT _DepthFormat)
{
	Ezptr<RenderTarget> Target = FindRenderTarget(_Name);

	if (Target != nullptr)
	{
		tassertmsg(true, "Overlapped RenderTargetName");
		return false;
	}

	Target = new RenderTarget;

	if (false == Target->CreateRenderTarget(_Name, _Width, _Height, _Format, _ClearColor, _DepthFormat))
	{
		tassertmsg(true, "Failed To Create RenderTarget");
		return false;
	}

	RenderTargetMap.insert(std::unordered_map<std::string, Ezptr<RenderTarget>>::value_type(_Name, Target));

	return true;

}

bool RenderManager::OnDebugRenderTarget(const std::string & _Name, const Vec3 & _Pos, const Vec3 & _Scale)
{
	Ezptr<RenderTarget> Target = FindRenderTarget(_Name);

	if (Target == nullptr)
	{
		tassertmsg(true, "Unexist RenderTarget");
		return false;
	}

	Target->OnDebug(_Pos, _Scale);

	return true;
}

bool RenderManager::AddMultiRenderTarget(const std::string & _Name, const std::string & _Target)
{
	Ezptr<MultiRenderTarget> MRT = FindMultiRenderTarget(_Name);

	if (MRT == nullptr)
	{
		MRT = new MultiRenderTarget;
		MultiRenderTargetMap.insert(std::unordered_map<std::string, Ezptr<MultiRenderTarget>>::value_type(_Name, MRT));
	}

	if (MRT->RenderTargetVec.size() == 8)
	{
		return false;
	}

	Ezptr<RenderTarget> Target = FindRenderTarget(_Target);

	MRT->RenderTargetVec.push_back(Target);

	return true;
}

bool RenderManager::AddDepth(const std::string & _Name, const std::string & _Target)
{
	Ezptr<MultiRenderTarget> MRT = FindMultiRenderTarget(_Name);

	if (MRT == nullptr)
	{
		MRT = new MultiRenderTarget;
		MultiRenderTargetMap.insert(std::unordered_map<std::string, Ezptr<MultiRenderTarget>>::value_type(_Name, MRT));
	}

	Ezptr<RenderTarget> Target = FindRenderTarget(_Target);

	MRT->DepthView = Target->GetDepthView();
	
	return true;
}

void RenderManager::ClearMultiRenderTarget(const std::string & _Name)
{
	Ezptr<MultiRenderTarget> MRT = FindMultiRenderTarget(_Name);

	if (MRT == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < MRT->RenderTargetVec.size(); i++)
	{
		MRT->RenderTargetVec[i]->ClearTarget();
	}
}

void RenderManager::SetMultiRenderTarget(const std::string & _Name)
{
	Ezptr<MultiRenderTarget> MRT = FindMultiRenderTarget(_Name);

	if (MRT == nullptr)
	{
		return;
	}

	if (MRT->OldTargetVec.size() < MRT->RenderTargetVec.size())
	{
		MRT->OldTargetVec.clear();
		MRT->OldTargetVec.resize(MRT->RenderTargetVec.size());
	}

	GetAIMContext->OMGetRenderTargets(MRT->RenderTargetVec.size(), &MRT->OldTargetVec[0], &MRT->OldDepthView);

	std::vector<ID3D11RenderTargetView*> TargetVec;
	TargetVec.resize(MRT->RenderTargetVec.size());

	for (size_t i = 0; i < MRT->RenderTargetVec.size(); i++)
	{
		TargetVec[i] = MRT->RenderTargetVec[i]->GetRTV();
	}

	ID3D11DepthStencilView* Depth = MRT->DepthView;

	if (Depth == nullptr)
	{
		Depth = MRT->OldDepthView;
	}

	GetAIMContext->OMSetRenderTargets(MRT->RenderTargetVec.size(), &TargetVec[0], Depth);
}

void RenderManager::ResetMultiRenderTarget(const std::string & _Name)
{
	Ezptr<MultiRenderTarget> MRT = FindMultiRenderTarget(_Name);

	if (MRT == nullptr)
	{
		return;
	}

	GetAIMContext->OMSetRenderTargets(MRT->RenderTargetVec.size(), &MRT->OldTargetVec[0], MRT->OldDepthView);

	if (MRT->OldDepthView != nullptr)
	{
		MRT->OldDepthView->Release();
		MRT->OldDepthView = nullptr;
	}

	for (size_t i = 0; i < MRT->OldTargetVec.size(); i++)
	{
		if (MRT->OldTargetVec[i] != nullptr)
		{
			MRT->OldTargetVec[i]->Release();
			MRT->OldTargetVec[i] = nullptr;
		}
	}
}

Ezptr<RenderState> RenderManager::FindRenderState(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<RenderState>>::iterator FindIter = RenderStateMap.find(_Name);

	if (FindIter == RenderStateMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

Ezptr<RenderTarget> RenderManager::FindRenderTarget(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<RenderTarget>>::iterator FindIter = RenderTargetMap.find(_Name);

	if (FindIter == RenderTargetMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

Ezptr<MultiRenderTarget> RenderManager::FindMultiRenderTarget(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<MultiRenderTarget>>::iterator FindIter = MultiRenderTargetMap.find(_Name);

	if (FindIter == MultiRenderTargetMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void RenderManager::RenderForward(float _Time)
{
	for (int AA = 0; AA < RG_END; ++AA)
	{
		for (int BB = 0; BB < RenderGroupList[AA].Size; ++BB)
		{
			RenderGroupList[AA].ObjList[BB]->Render(_Time);
		}

		RenderGroupList[AA].Size = 0;
	}
}

void RenderManager::RenderDeferred(float _Time)
{
	RenderGBuffer(_Time);

	for (int i = 0; i < RG_END; i++)
	{
		RenderGroupList[i].Size = 0;
	}
}

void RenderManager::RenderGBuffer(float _Time)
{
	ClearMultiRenderTarget("GBuffer");

	SetMultiRenderTarget("GBuffer");

	for (int AA = RG_LANDSCAPE; AA <= RG_DEFAULT; ++AA)
	{
		for (int BB = 0; BB < RenderGroupList[AA].Size; ++BB)
		{
			RenderGroupList[AA].ObjList[BB]->Render(_Time);
		}
	}

	ResetMultiRenderTarget("GBuffer");
}
