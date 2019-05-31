#include "RenderManager.h"
#include "ShaderManager.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "AIMLight.h"
#include "AIMDevice.h"
#include "ResourceManager.h"
#include "AIMShader.h"
#include "AIMMesh.h"
#include "CollisionManager.h"
#include "AIMMaterial.h"
#include "AIMRenderer.h"
#include "AIMTransform.h"
#include "AIMCamera.h"
#include "AIMScene.h"

std::unordered_map<std::string, Ezptr<RenderState>> RenderManager::RenderStateMap;
std::unordered_map<std::string, Ezptr<RenderTarget>> RenderManager::RenderTargetMap;
std::unordered_map<std::string, Ezptr<MultiRenderTarget>> RenderManager::MultiRenderTargetMap;
RenderList RenderManager::LightList;
RenderList RenderManager::RenderGroupList[RG_END];
RenderMode RenderManager::RM = RM_DEFERRED;
RenderConstBuffer RenderManager::RenderCBuffer;
Ezptr<RenderState> RenderManager::DepthDisable;
Ezptr<RenderState> RenderManager::AccBlend;
Ezptr<AIMShader> RenderManager::LightAccDirShader;
Ezptr<RenderState> RenderManager::AlphaBlend;
Ezptr<AIMShader> RenderManager::LightBlendShader;
Ezptr<AIMShader> RenderManager::LightBlendRenderShader;
Ezptr<RenderState> RenderManager::CullNone;
Ezptr<AIMShader> RenderManager::LightAccPointShader;
Ezptr<AIMShader> RenderManager::LightAccSpotShader;
Ezptr<AIMMesh> RenderManager::LightPointVolume;
ID3D11InputLayout* RenderManager::LightPointLayout = nullptr;

std::unordered_map<unsigned __int64, InstancingGeometry*> RenderManager::InstancingGeometryMap;
std::list<InstancingGeometry*> RenderManager::InstancingList[RG_END];
InstancingBuffer* RenderManager::StaticInstancing = nullptr;
InstancingBuffer* RenderManager::FrameAnimInstancing = nullptr;
InstancingBuffer* RenderManager::AnimInstancing = nullptr;
InstancingBuffer* RenderManager::ColliderInstancing = nullptr;
InstancingBuffer* RenderManager::LightInstancing = nullptr;

Ezptr<AIMShader> RenderManager::StaticInstancingShader;
Ezptr<AIMShader> RenderManager::FrameAnimInstancingShader;
Ezptr<AIMShader> RenderManager::AnimInstancingShader;

ID3D11InputLayout* RenderManager::StaticInstancingLayout = nullptr;
ID3D11InputLayout* RenderManager::FrameAnimInstancingLayout = nullptr;
ID3D11InputLayout* RenderManager::AnimInstancingLayout = nullptr;


InstancingGeometry * RenderManager::FindInstancingGeometry(unsigned __int64 _Key)
{
	std::unordered_map<unsigned __int64, InstancingGeometry*>::iterator FindIter = InstancingGeometryMap.find(_Key);

	if (FindIter == InstancingGeometryMap.end())
	{
		return nullptr;
	}
	return FindIter->second;
}

InstancingBuffer* RenderManager::CreateInstancingBuffer(int _Size, int _Count)
{
	InstancingBuffer* Buffer = new InstancingBuffer;

	Buffer->Size = _Size;
	Buffer->Count = _Count;
	Buffer->Usage = D3D11_USAGE_DYNAMIC;
	Buffer->Data = new char[_Size * _Count];
	
	D3D11_BUFFER_DESC Desc = {};
	Desc.ByteWidth = _Size * _Count;
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(GetAIMDevice->CreateBuffer(&Desc, nullptr, &Buffer->Buffer)))
	{
		return nullptr;
	}

	return Buffer;
}

void RenderManager::ResizeInstancingBuffer(InstancingBuffer* _Buffer, int _Count)
{
	if (_Buffer->Data != nullptr)
	{
		delete[] _Buffer->Data;
	}

	if (_Buffer->Buffer != nullptr)
	{
		delete _Buffer->Buffer;
	}

	_Buffer->Count = _Count;
	_Buffer->Data = new char[_Buffer->Size * _Count];

	D3D11_BUFFER_DESC Desc = {};
	Desc.ByteWidth = _Buffer->Size * _Count;
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(GetAIMDevice->CreateBuffer(&Desc, nullptr, &_Buffer->Buffer)))
	{
		return;
	}
}

void RenderManager::AddInstancingData(InstancingBuffer* _Buffer, int _Pos, void * _Data)
{
	char* BufferData = (char*)_Buffer->Data;
	memcpy(BufferData + (_Pos * _Buffer->Size), _Data, _Buffer->Size);
}

void RenderManager::CopyInstancingData(InstancingBuffer* _Buffer, int _Cout)
{
	D3D11_MAPPED_SUBRESOURCE Map = {};

	GetAIMContext->Map(_Buffer->Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

	memcpy(Map.pData, _Buffer->Data, _Buffer->Size * _Cout);

	GetAIMContext->Unmap(_Buffer->Buffer, 0);
}

Ezptr<AIMLight> RenderManager::GetFirstLight()
{
	if (LightList.Size == 0)
	{
		return nullptr;
	}
	else if (RM == RM_DEFERRED)
	{
		return nullptr;
	}

	return LightList.ObjList[0]->FindComponent<AIMLight>(CT_LIGHT);
}

bool RenderManager::Init()
{
	StaticInstancing = CreateInstancingBuffer(sizeof(InstancingStaticBuffer));
	FrameAnimInstancing = CreateInstancingBuffer(sizeof(InstancingFrameAnimBuffer));
	AnimInstancing = CreateInstancingBuffer(sizeof(InstancingStaticBuffer));

	RenderCBuffer.RenderMode = RM;
	
	if (false == ShaderManager::Init())
	{
		tassertmsg(true, "ShaderManger Init Failed");
		return false;
	}

	StaticInstancingShader = ShaderManager::FindShader("Standard3DInstancingShader");
	StaticInstancingLayout = ShaderManager::FindInputLayout("Standard3DInstancingLayout");

	CreateRasterizerState("CullNone", D3D11_FILL_SOLID, D3D11_CULL_NONE);

	CreateRasterizerState("WireFrame", D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);

	CreateDepthState("LessEqual", TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);

	CreateDepthState("DepthDisable", FALSE);
	
	AddTargetBlendDesc("AlphaBlend", TRUE, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA);
	
	CreateBlendState("AlphaBlend", TRUE, FALSE);

	AddTargetBlendDesc("AccBlend", TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE);

	CreateBlendState("AccBlend", TRUE, FALSE);

	CullNone = FindRenderState("CullNone");

	DepthDisable = FindRenderState("DepthDisable");
	AccBlend = FindRenderState("AccBlend");
	AlphaBlend = FindRenderState("AlphaBlend");
	LightAccDirShader = ShaderManager::FindShader("LightAccDirShader");
	LightAccPointShader = ShaderManager::FindShader("LightAccPointShader");
	LightAccSpotShader = ShaderManager::FindShader("LightAccSpotShader");

	LightPointLayout = ShaderManager::FindInputLayout("StandardPosLayout");
	LightPointVolume = ResourceManager::FindMesh("Sky");
	LightBlendShader = ShaderManager::FindShader("LightBlendShader");
	LightBlendRenderShader = ShaderManager::FindShader("LightBlendRenderShader");

	float ClearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	if (false == CreateRenderTarget("Albedo", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor, 8))
	{
		return false;
	}

	OnDebugRenderTarget("Albedo", Vec3(0.0f, 0.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("Normal", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor, 8))
	{
		return false;
	}

	OnDebugRenderTarget("Normal", Vec3(0.0f, 100.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("Depth", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor, 8))
	{
		return false;
	}

	OnDebugRenderTarget("Depth", Vec3(0.0f, 200.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("MaterialDif", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor, 8))
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

	if (false == CreateRenderTarget("LightDiffuse", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("LightDiffuse", Vec3(100.0, 0.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("LightSpecular", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("LightSpecular", Vec3(100.0, 100.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	if (false == CreateRenderTarget("LightBlend", GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, DXGI_FORMAT_R32G32B32A32_FLOAT, ClearColor))
	{
		return false;
	}

	OnDebugRenderTarget("LightBlend", Vec3(200.0, 0.0f, 0.0f), Vec3(100.0f, 100.0f, 1.0f));

	AddMultiRenderTarget("LightAcc", "LightDiffuse");
	AddMultiRenderTarget("LightAcc", "LightSpecular");


	return true;
}

void RenderManager::Release()
{
	std::unordered_map<unsigned __int64, InstancingGeometry*>::iterator StartIter = InstancingGeometryMap.begin();
	std::unordered_map<unsigned __int64, InstancingGeometry*>::iterator EndIter = InstancingGeometryMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (StartIter->second != nullptr)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	if (StaticInstancing != nullptr)
	{
		if (StaticInstancing->Data != nullptr)
		{
			delete[] StaticInstancing->Data;
			StaticInstancing->Data = nullptr;
		}

		if (StaticInstancing->Buffer != nullptr)
		{
			StaticInstancing->Buffer->Release();
			StaticInstancing->Buffer = nullptr;
		}

		delete StaticInstancing;
		StaticInstancing = nullptr;
	}

	if (FrameAnimInstancing != nullptr)
	{
		if (FrameAnimInstancing->Data != nullptr)
		{
			delete[] FrameAnimInstancing->Data;
			FrameAnimInstancing->Data = nullptr;
		}

		if (FrameAnimInstancing->Buffer != nullptr)
		{
			FrameAnimInstancing->Buffer->Release();
			FrameAnimInstancing->Buffer = nullptr;
		}

		delete FrameAnimInstancing;
		FrameAnimInstancing = nullptr;
	}

	if (AnimInstancing != nullptr)
	{
		if (AnimInstancing->Data != nullptr)
		{
			delete[] AnimInstancing->Data;
			AnimInstancing->Data = nullptr;
		}

		if (AnimInstancing->Buffer != nullptr)
		{
			AnimInstancing->Buffer->Release();
			AnimInstancing->Buffer = nullptr;
		}

		delete AnimInstancing;
		AnimInstancing = nullptr;
	}

	if (ColliderInstancing != nullptr)
	{
		if (ColliderInstancing->Data != nullptr)
		{
			delete[] ColliderInstancing->Data;
			ColliderInstancing->Data = nullptr;
		}

		if (ColliderInstancing->Buffer != nullptr)
		{
			ColliderInstancing->Buffer->Release();
			ColliderInstancing->Buffer = nullptr;
		}

		delete ColliderInstancing;
		ColliderInstancing = nullptr;
	}

	if (LightInstancing != nullptr)
	{
		if (LightInstancing->Data != nullptr)
		{
			delete[] LightInstancing->Data;
			LightInstancing->Data = nullptr;
		}

		if (LightInstancing->Buffer != nullptr)
		{
			LightInstancing->Buffer->Release();
			LightInstancing->Buffer = nullptr;
		}

		delete LightInstancing;
		LightInstancing = nullptr;
	}

	StaticInstancingShader = nullptr;
	FrameAnimInstancingShader = nullptr;
	AnimInstancingShader = nullptr;

	CullNone = nullptr;
	LightAccPointShader = nullptr;
	LightAccSpotShader = nullptr;
	LightPointVolume = nullptr;
	AlphaBlend = nullptr;
	LightBlendShader = nullptr;
	LightBlendRenderShader = nullptr;
	AccBlend = nullptr;
	LightAccDirShader = nullptr;
	DepthDisable = nullptr;
	MultiRenderTargetMap.clear();
	RenderStateMap.clear();
	RenderTargetMap.clear();
	ShaderManager::Release();
}

void RenderManager::AddRenderObject(Ezptr<AIMObject> _Obj)
{
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
		if (false == _Obj->CheckComponent(CT_TEXT))
		{
			return;
		}
	}

	RenderGroup Tmp = _Obj->GetRenderGroup();

	Ezptr<AIMRenderer> Renderer = _Obj->FindComponent<AIMRenderer>(CT_RENDERER);

	if (Renderer != nullptr)
	{
		Ezptr<AIMMesh> Mesh = Renderer->GetMesh();
		Ezptr<AIMMaterial> Material = _Obj->FindComponent<AIMMaterial>(CT_MATERIAL);

		if (Tmp <= RG_ALPHA)
		{
			Vec3 Center = Mesh->GetCenter();
			float Radius = Mesh->GetRadius();

			Ezptr<AIMTransform> Transform = _Obj->GetTransform();

			Vec3 Pos = Transform->GetWorldPosition();
			Vec3 Scale = Transform->GetWorldScale();

			Center *= Scale;
			Center += Pos;

			float fScale = Scale.x;
			fScale = fScale < Scale.y ? Scale.y : fScale;
			fScale = fScale < Scale.z ? Scale.z : fScale;

			Radius *= fScale;

			Ezptr<AIMCamera> MainCamera = _Obj->GetScene()->GetMainCamera();
			bool FrustrumCull = MainCamera->FrustrumInSphere(Center, Radius);

			_Obj->SetFrustrumCulling(!FrustrumCull);

			if (FrustrumCull == false)
			{
				return;
			}
			else
			{
				// UI
			}
		}

		unsigned int MeshNumber = Mesh->GetSerialNumber();
		unsigned int MtrlNumber = Material->GetSerialNumber();

		unsigned __int64 Key = MeshNumber;

		Key <<= 32;
		Key |= MtrlNumber;

		InstancingGeometry* Geometry = FindInstancingGeometry(Key);

		if (Geometry == nullptr)
		{
			Geometry = new InstancingGeometry;
			InstancingGeometryMap.insert(std::unordered_map<unsigned __int64, InstancingGeometry*>::value_type(Key, Geometry));
		}

		if (_Obj->CheckComponent(CT_FRAMEANIMATION) == true)
		{
			Geometry->FrameAnimation = true;
		}
		else if (_Obj->CheckComponent(CT_ANIMATION) == true)
		{
			Geometry->Animation = true;
		}
		else
		{
			Geometry->Animation = false;
			Geometry->FrameAnimation = false;
		}

		Geometry->Add(_Obj);
	}

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

void RenderManager::ComputeInstancing()
{
	for (int i = 0; i < RG_END; i++)
	{
		InstancingList[i].clear();
	}

	std::unordered_map<unsigned __int64, InstancingGeometry*>::iterator StartIter = InstancingGeometryMap.begin();
	std::unordered_map<unsigned __int64, InstancingGeometry*>::iterator EndIter = InstancingGeometryMap.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		InstancingGeometry* Geometry = StartIter->second;

		if (Geometry->Size >= INSTANCING_COUNT)
		{
			RenderGroup Tmp = Geometry->ObjList[0]->GetRenderGroup();

			if (Geometry->Animation == true)
			{
				if (Geometry->Size > AnimInstancing->Count)
				{
					ResizeInstancingBuffer(AnimInstancing, Geometry->Size);
				}
			}

			else if (Geometry->FrameAnimation == true)
			{
				if (Geometry->Size > FrameAnimInstancing->Count)
				{
					ResizeInstancingBuffer(FrameAnimInstancing, Geometry->Size);
				}
			}

			else
			{
				if (Geometry->Size > StaticInstancing->Count)
				{
					ResizeInstancingBuffer(StaticInstancing, Geometry->Size);
				}
			}

			for (int i = 0; i < Geometry->Size; i++)
			{
				Geometry->ObjList[i]->SetInstancingEnable(true);
			}

			InstancingList[Tmp].push_back(Geometry);
		}
	}
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

	DepthDisable->SetState();

	std::unordered_map<std::string, Ezptr<RenderTarget>>::iterator StartIter = RenderTargetMap.begin();
	std::unordered_map<std::string, Ezptr<RenderTarget>>::iterator EndIter = RenderTargetMap.end();

	for (; StartIter != EndIter  ; ++StartIter)
	{
		StartIter->second->Render();
	}

	DepthDisable->ResetState();

	LightList.Size = 0;

	std::unordered_map<unsigned __int64, InstancingGeometry*>::iterator StartIter2 = InstancingGeometryMap.begin();
	std::unordered_map<unsigned __int64, InstancingGeometry*>::iterator EndIter2 = InstancingGeometryMap.end();

	for (; StartIter2 != EndIter2 ; ++StartIter2)
	{
		StartIter2->second->Clear();
	}
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

bool RenderManager::AddTargetBlendDesc(const std::string & _Name, BOOL _Enable, D3D11_BLEND _SrcBlend, D3D11_BLEND _DestBlend, D3D11_BLEND_OP _Op, D3D11_BLEND _SrcBlendAlpha, D3D11_BLEND _DestBlendAlpha, D3D11_BLEND_OP _AlphaOp, UINT _WriteMask)
{
	Ezptr<BlendState> State = FindRenderState(_Name);

	if (State == nullptr)
	{
		State = new BlendState;
		RenderStateMap.insert(std::unordered_map<std::string, Ezptr<RenderState>>::value_type(_Name, State));
	}

	State->AddTargetBlendDesc(_Enable, _SrcBlend, _DestBlend, _Op, _SrcBlendAlpha, _DestBlendAlpha, _AlphaOp, _WriteMask);

	return true;
}

bool RenderManager::CreateBlendState(const std::string & _Name, BOOL _AlphaToCoverage, BOOL _IndependentBlend)
{
	Ezptr<BlendState> State = FindRenderState(_Name);

	if (State == nullptr)
	{
		tassertmsg(true, "BlendState Name Unexist");
		return false;
	}

	if (false == State->CreateState(_Name, _AlphaToCoverage, _IndependentBlend))
	{
		std::unordered_map<std::string, Ezptr<RenderState>>::iterator FindIter = RenderStateMap.find(_Name);

		RenderStateMap.erase(FindIter);

		tassertmsg(true, "Failed To Create BlendState");
		return false;
	}

	return true;
}

bool RenderManager::CreateRenderTarget(const std::string & _Name, UINT _Width, UINT _Height, DXGI_FORMAT _Format, float _ClearColor[4], int _SamplerCount, DXGI_FORMAT _DepthFormat)
{
	Ezptr<RenderTarget> Target = FindRenderTarget(_Name);

	if (Target != nullptr)
	{
		tassertmsg(true, "Overlapped RenderTargetName");
		return false;
	}

	Target = new RenderTarget;

	if (false == Target->CreateRenderTarget(_Name, _Width, _Height, _Format, _ClearColor, _SamplerCount, _DepthFormat))
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

	GetAIMContext->OMGetRenderTargets((UINT)MRT->RenderTargetVec.size(), &MRT->OldTargetVec[0], &MRT->OldDepthView);

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

	GetAIMContext->OMSetRenderTargets((UINT)MRT->RenderTargetVec.size(), &TargetVec[0], Depth);
}

void RenderManager::ResetMultiRenderTarget(const std::string & _Name)
{
	Ezptr<MultiRenderTarget> MRT = FindMultiRenderTarget(_Name);

	if (MRT == nullptr)
	{
		return;
	}

	GetAIMContext->OMSetRenderTargets((UINT)MRT->RenderTargetVec.size(), &MRT->OldTargetVec[0], MRT->OldDepthView);

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

		if (AA == RG_DEFAULT)
		{
			CollisionManager::Render(_Time);
		}

		RenderGroupList[AA].Size = 0;
	}
}

void RenderManager::RenderDeferred(float _Time)
{
	// GBuffer 그리고
	RenderGBuffer(_Time);

	// 조명 누적 버퍼 생성
	RenderLightAcc(_Time);

	// 조명누적버퍼 + Albedo합친 최종조명처리 타겟
	RenderLightBlend(_Time);

	// 조명이 합성된 최종타겟
	RenderLightBlendRender(_Time);

	CollisionManager::Render(_Time);

	Ezptr<RenderTarget> DepthTarget = FindRenderTarget("Depth");

	DepthTarget->SetShader(11);

	for (int i = 0; i < RenderGroupList[RG_ALPHA].Size; i++)
	{
		RenderGroupList[RG_ALPHA].ObjList[i]->Render(_Time);
	}

	DepthTarget->ResetShader(11);

	for (int AA = RG_HUD; AA < RG_END; AA++)
	{
		for (int BB = 0; BB < RenderGroupList[AA].Size; BB++)
		{
			RenderGroupList[AA].ObjList[BB]->Render(_Time);
		}
	}

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
		std::list<InstancingGeometry*>::iterator StartIter = InstancingList[AA].begin();
		std::list<InstancingGeometry*>::iterator EndIter = InstancingList[AA].end();

		for (; StartIter != EndIter; ++StartIter)
		{
			Ezptr<AIMShader> Shader;
			ID3D11InputLayout* Layout = nullptr;

			InstancingGeometry* Geometry = *StartIter;
			for (int BB = 0; BB < Geometry->Size; BB++)
			{
				AIMObject* Obj = Geometry->ObjList[BB];

				Ezptr<AIMTransform> Transform = Obj->GetTransform();
				Ezptr<AIMScene> Scene = Obj->GetScene();
				Ezptr<AIMCamera> Camera = Scene->GetMainCamera();

				if (Geometry->Animation == true)
				{

				}
				else if (Geometry->FrameAnimation == true)
				{

				}
				else
				{
					Matrix WVP, WV, WVRot;
					WV = Transform->GetLocalMatrix() * Transform->GetWorldMatrix() * Camera->GetView();
					WVP = WV * Camera->GetProjection();

					WVRot = Transform->GetLocalRotationMatrix() * Transform->GetWorldRotationMatrix() * Camera->GetView();

					InstancingStaticBuffer Buffer;
					Buffer.WV = WV;
					Buffer.WVP = WVP;
					Buffer.WVRot = WVRot;

					Buffer.WV.Transpose();
					Buffer.WVP.Transpose();
					Buffer.WVRot.Transpose();

					AddInstancingData(StaticInstancing, BB, &Buffer);

					Shader = StaticInstancingShader;
					Layout = StaticInstancingLayout;
				}
			}

			InstancingBuffer* Buffer = nullptr;

			if (Geometry->Animation == true)
			{
				Buffer = AnimInstancing;
			}
			else if (Geometry->FrameAnimation == true)
			{
				Buffer = FrameAnimInstancing;
			}
			else
			{
				Buffer = StaticInstancing;
			}

			CopyInstancingData(Buffer, Geometry->Size);

			Geometry->ObjList[0]->PrevRender(_Time);

			Ezptr<AIMRenderer> Renderer = Geometry->ObjList[0]->FindComponent<AIMRenderer>(CT_RENDERER);

			Renderer->RenderInstancing(Buffer, Shader, Layout, Geometry->Size, _Time);
		}
	}

	for (int AA = RG_LANDSCAPE; AA <= RG_DEFAULT; AA++)
	{
		for (int BB = 0; BB < RenderGroupList[AA].Size; BB++)
		{
			RenderGroupList[AA].ObjList[BB]->Render(_Time);
		}
	}

	ResetMultiRenderTarget("GBuffer");
}

void RenderManager::RenderLightAcc(float _Time)
{
	ClearMultiRenderTarget("LightAcc");

	SetMultiRenderTarget("LightAcc");

	DepthDisable->SetState();
	AccBlend->SetState();

	Ezptr<MultiRenderTarget> GBuf = FindMultiRenderTarget("GBuffer");

	GBuf->RenderTargetVec[1]->SetShader(11);
	GBuf->RenderTargetVec[2]->SetShader(12);
	GBuf->RenderTargetVec[3]->SetShader(13);
	GBuf->RenderTargetVec[4]->SetShader(14);
	GBuf->RenderTargetVec[5]->SetShader(15);
	GBuf->RenderTargetVec[6]->SetShader(16);

	for (int i = 0; i < LightList.Size; i++)
	{
		Ezptr<AIMLight> Light = LightList.ObjList[i]->FindComponent<AIMLight>(CT_LIGHT);
		
		switch (Light->GetLightInfo().Type)
		{
		case LT_DIR:
			RenderLightDir(_Time, Light);
			break;
		case LT_POINT:
			RenderLightPoint(_Time, Light);
			break;
		case LT_SPOT:
			RenderLightSpot(_Time, Light);
			break;
		}
	}

	GBuf->RenderTargetVec[1]->ResetShader(11);
	GBuf->RenderTargetVec[2]->ResetShader(12);
	GBuf->RenderTargetVec[3]->ResetShader(13);
	GBuf->RenderTargetVec[4]->ResetShader(14);
	GBuf->RenderTargetVec[5]->ResetShader(15);
	GBuf->RenderTargetVec[6]->ResetShader(16);

	DepthDisable->ResetState();
	AccBlend->ResetState();

	ResetMultiRenderTarget("LightAcc");
}

void RenderManager::RenderLightDir(float _Time, Ezptr<AIMLight> _Light)
{
	LightAccDirShader->SetShader();

	_Light->SetShader();

	// 전체 화면크기의 사각형 출력
	GetAIMContext->IASetInputLayout(nullptr);

	UINT Offset = 0;
	GetAIMContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	GetAIMContext->IASetVertexBuffers(0, 0, nullptr, 0, &Offset);
	GetAIMContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	GetAIMContext->Draw(4, 0);
}

void RenderManager::RenderLightPoint(float _Time, Ezptr<AIMLight> _Light)
{
	LightAccPointShader->SetShader();

	_Light->SetShader();

	GetAIMContext->IASetInputLayout(LightPointLayout);

	CullNone->SetState();

	LightPointVolume->Render();

	CullNone->ResetState();

	// 전체 화면크기의 사각형 출력
	/*GetAIMContext->IASetInputLayout(nullptr);

	UINT Offset = 0;
	GetAIMContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	GetAIMContext->IASetVertexBuffers(0, 0, nullptr, 0, &Offset);
	GetAIMContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	GetAIMContext->Draw(4, 0);*/
}

void RenderManager::RenderLightSpot(float _Time, Ezptr<AIMLight> _Light)
{
	LightAccSpotShader->SetShader();

	_Light->SetShader();

	// 전체 화면크기의 사각형 출력
	GetAIMContext->IASetInputLayout(nullptr);

	UINT Offset = 0;
	GetAIMContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	GetAIMContext->IASetVertexBuffers(0, 0, nullptr, 0, &Offset);
	GetAIMContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	GetAIMContext->Draw(4, 0);
}

void RenderManager::RenderLightBlend(float _Time)
{
	Ezptr<RenderTarget> LightBlendTarget = FindRenderTarget("LightBlend");

	LightBlendTarget->ClearTarget();

	LightBlendTarget->SetTarget();

	LightBlendShader->SetShader();
	
	DepthDisable->SetState();

	Ezptr<MultiRenderTarget> LightAcc = FindMultiRenderTarget("LightAcc");
	Ezptr<RenderTarget> Albedo = FindRenderTarget("Albedo");

	Albedo->SetShader(10);
	LightAcc->RenderTargetVec[0]->SetShader(17);
	LightAcc->RenderTargetVec[1]->SetShader(18);

	GetAIMContext->IASetInputLayout(nullptr);

	UINT Offset = 0;
	GetAIMContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	GetAIMContext->IASetVertexBuffers(0, 0, nullptr, 0, &Offset);
	GetAIMContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	GetAIMContext->Draw(4, 0);

	Albedo->ResetShader(10);
	LightAcc->RenderTargetVec[0]->ResetShader(17);
	LightAcc->RenderTargetVec[1]->ResetShader(18);

	DepthDisable->ResetState();

	LightBlendTarget->ResetTarget();
}

void RenderManager::RenderLightBlendRender(float _Time)
{
	LightBlendRenderShader->SetShader();

	DepthDisable->SetState();
	AlphaBlend->SetState();

	Ezptr<RenderTarget> LightBlend = FindRenderTarget("LightBlend");

	LightBlend->SetShader(0);

	GetAIMContext->IASetInputLayout(nullptr);

	UINT Offset = 0;
	GetAIMContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	GetAIMContext->IASetVertexBuffers(0, 0, nullptr, 0, &Offset);
	GetAIMContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	GetAIMContext->Draw(4, 0);

	LightBlend->ResetShader(0);

	AlphaBlend->ResetState();
	DepthDisable->ResetState();
}
