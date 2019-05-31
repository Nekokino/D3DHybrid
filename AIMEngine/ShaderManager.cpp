#include "ShaderManager.h"
#include "AIMShader.h"
#include "AIMDevice.h"

std::unordered_map<std::string, Ezptr<AIMShader>> ShaderManager::ShaderMap;
std::unordered_map<std::string, ID3D11InputLayout*> ShaderManager::InputLayoutMap;
std::unordered_map<std::string, Ezptr<ConstBuffer>> ShaderManager::ConstBufferMap;

std::vector<D3D11_INPUT_ELEMENT_DESC> ShaderManager::InputDescVec;
UINT ShaderManager::InputSize = 0;

bool ShaderManager::Init()
{
	std::string Entry[ST_END] = {};

#pragma region StandardColorShader

	Entry[ST_VTX] = "StandardColorVS";
	Entry[ST_PIX] = "StandardColorPS";

	if (false == LoadShader("StandardColorShader", TEXT("Standard.fx"), Entry, "Shader"))
	{
		return false;
	}

	// 입력 레이아웃 생성 과정
	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (false == CreateInputLayout("StandardColorLayout", "StandardColorShader"))
	{
		return false;
	}

#pragma endregion

#pragma region StandardColorNormalShader

	Entry[ST_VTX] = "StandardColorNormalVS";
	Entry[ST_PIX] = "StandardColorNormalPS";

	if (false == LoadShader("StandardColorNormalShader", TEXT("Standard.fx"), Entry, "Shader"))
	{
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (false == CreateInputLayout("StandardColorNormalLayout", "StandardColorNormalShader"))
	{
		return false;
	}

#pragma endregion

#pragma region SkyShader

	Entry[ST_VTX] = "SkyVS";
	Entry[ST_PIX] = "SkyPS";

	if (false == LoadShader("SkyShader", TEXT("Standard.fx"), Entry, "Shader"))
	{
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (false == CreateInputLayout("StandardPosLayout", "SkyShader"))
	{
		return false;
	}

#pragma endregion

#pragma region StandardVertex3DShader

	Entry[ST_VTX] = "StandardVertex3DVS";
	Entry[ST_PIX] = "StandardVertex3DPS";

	if (false == LoadShader("Vertex3DShader", TEXT("Standard.fx"), Entry, "Shader"))
	{
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 8, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (false == CreateInputLayout("StandardVertex3DLayout", "Vertex3DShader"))
	{
		return false;
	}

#pragma endregion

#pragma region StandardTexNormalShader

	Entry[ST_VTX] = "StandardTexNormalVS";
	Entry[ST_PIX] = "Standard3DPS";

	if (false == LoadShader("StandardTexNormalShader", TEXT("Standard.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region Standard3DShader

	Entry[ST_VTX] = "Standard3DVS";
	Entry[ST_PIX] = "Standard3DPS";

	if (false == LoadShader("Standard3DShader", TEXT("Standard.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region StandardUVShader

	Entry[ST_VTX] = "DebugVS";
	Entry[ST_PIX] = "DebugPS";

	if (false == LoadShader("DebugShader", TEXT("Debug.fx"), Entry, "Shader"))
	{
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 8, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (false == CreateInputLayout("StandardUVLayout", "DebugShader"))
	{
		return false;
	}

#pragma endregion

#pragma region LightAccDirShader

	Entry[ST_VTX] = "LightAccDirVS";
	Entry[ST_PIX] = "LightAccPS";

	if (false == LoadShader("LightAccDirShader", TEXT("Light.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region LightAccPointShader

	Entry[ST_VTX] = "LightAccPointVS";
	Entry[ST_PIX] = "LightAccPS";

	if (false == LoadShader("LightAccPointShader", TEXT("Light.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region LightAccSpotShader

	Entry[ST_VTX] = "LightAccSpotVS";
	Entry[ST_PIX] = "LightAccPS";

	if (false == LoadShader("LightAccSpotShader", TEXT("Light.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region LightBlendShader

	Entry[ST_VTX] = "LightAccDirVS";
	Entry[ST_PIX] = "LightBlendPS";

	if (false == LoadShader("LightBlendShader", TEXT("Light.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region LightBlendRenderShader

	Entry[ST_VTX] = "LightAccDirVS";
	Entry[ST_PIX] = "LightBlendRenderPS";

	if (false == LoadShader("LightBlendRenderShader", TEXT("Light.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region ColliderShader

	Entry[ST_VTX] = "ColliderVS";
	Entry[ST_PIX] = "ColliderPS";

	if (false == LoadShader("ColliderShader", TEXT("Collider.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region ParticleShader

	Entry[ST_VTX] = "ParticleVS";
	Entry[ST_PIX] = "ParticlePS";
	Entry[ST_GEO] = "ParticleGS";

	if (false == LoadShader("ParticleShader", TEXT("Particle.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region LandScapeShader

	Entry[ST_VTX] = "LandScapeVS";
	Entry[ST_PIX] = "LandScapePS";
	Entry[ST_GEO] = "";

	if (false == LoadShader("LandScapeShader", TEXT("LandScape.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion
	
#pragma region UIButtonShader

	Entry[ST_VTX] = "ButtonVS";
	Entry[ST_PIX] = "ButtonPS";
	Entry[ST_GEO] = "";

	if (false == LoadShader("ButtonShader", TEXT("UI.fx"), Entry, "Shader"))
	{
		return false;
	}

#pragma endregion

#pragma region Standard3DInstancingShader

	Entry[ST_VTX] = "Standard3DInstancingVS";
	Entry[ST_PIX] = "Standard3DInstancingPS";
	Entry[ST_GEO] = "";

	if (false == LoadShader("Standard3DInstancingShader", TEXT("Standard.fx"), Entry, "Shader"))
	{
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 8, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

	InputSize = 0;

	AddInputDesc("WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddInputDesc("WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddInputDesc("WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddInputDesc("WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	 
	AddInputDesc("WORLDVIEW", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddInputDesc("WORLDVIEW", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddInputDesc("WORLDVIEW", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddInputDesc("WORLDVIEW", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	AddInputDesc("WORLDVIEWROT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddInputDesc("WORLDVIEWROT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddInputDesc("WORLDVIEWROT", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddInputDesc("WORLDVIEWROT", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (false == CreateInputLayout("Standard3DInstancingLayout", "Standard3DInstancingShader"))
	{
		return false;
	}


#pragma endregion

#pragma region ConstBuffer

	CreateConstBuffer("Transform", sizeof(TransformConstBuffer), 0, CS_VERTEX | CS_PIXEL | CS_GEOMETRY);
	CreateConstBuffer("Material", sizeof(Material), 1, CS_VERTEX | CS_PIXEL);
	CreateConstBuffer("Light", sizeof(LightInfo), 2, CS_VERTEX | CS_PIXEL);
	CreateConstBuffer("Render", sizeof(RenderConstBuffer), 3, CS_VERTEX | CS_PIXEL);
	CreateConstBuffer("Debug", sizeof(DebugConstBuffer), 9, CS_VERTEX | CS_PIXEL);
	CreateConstBuffer("Collider", sizeof(Vec4), 10, CS_PIXEL);
	CreateConstBuffer("Particle", sizeof(ParticleConstBuffer), 10, CS_GEOMETRY);
	CreateConstBuffer("FrameAnimation", sizeof(FrameAnimationConstBuffer), 8, CS_VERTEX | CS_PIXEL | CS_GEOMETRY);
	CreateConstBuffer("LandScape", sizeof(LandScapeConstBuffer), 10, CS_VERTEX | CS_PIXEL);
	CreateConstBuffer("Button", sizeof(ButtonConstBuffer), 11, CS_VERTEX | CS_PIXEL);

#pragma endregion


	return true;
}


void ShaderManager::Release()
{
	ShaderMap.clear();

	std::unordered_map<std::string, ID3D11InputLayout*>::iterator StartIter = InputLayoutMap.begin();
	std::unordered_map<std::string, ID3D11InputLayout*>::iterator EndIter = InputLayoutMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (StartIter->second != nullptr)
		{
			StartIter->second->Release();
			StartIter->second = nullptr;
		}
	}

	InputLayoutMap.clear();
	ConstBufferMap.clear();
}


bool ShaderManager::LoadShader(const std::string & _Name, const TCHAR * _FileName, std::string _Entry[ST_END], const std::string & _PathKey)
{
	Ezptr<AIMShader> Shader = FindShader(_Name);

	if (nullptr != Shader)
	{
		tassertmsg(true, "Overlapped Shader Name");
		return false;
	}

	Shader = new AIMShader;

	if (false == Shader->LoadShader(_Name, _FileName, _Entry, _PathKey))
	{
		tassertmsg(true, "Load Shader Failed");
		return false;
	}

	ShaderMap.insert(std::unordered_map<std::string, Ezptr<AIMShader>>::value_type(_Name, Shader));

	return true;
}

Ezptr<AIMShader> ShaderManager::FindShader(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<AIMShader>>::iterator FindIter = ShaderMap.find(_Name);

	if (FindIter == ShaderMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void ShaderManager::AddInputDesc(const char * _Semantic, UINT _SemanticIdx, DXGI_FORMAT _Format, UINT _Size, UINT _InputSlot, D3D11_INPUT_CLASSIFICATION _Class, UINT _StepRate)
{
	D3D11_INPUT_ELEMENT_DESC Desc = {};

	Desc.SemanticName = _Semantic;
	Desc.SemanticIndex = _SemanticIdx;
	Desc.Format = _Format;
	Desc.InputSlot = _InputSlot;
	Desc.InputSlotClass = _Class;
	Desc.InstanceDataStepRate = _StepRate;
	Desc.AlignedByteOffset = InputSize;

	// 총 인풋 사이즈와 다음 인풋 위치를 결정해주기 위해서 누적으로 계산해준다.
	InputSize += _Size;

	InputDescVec.push_back(Desc);
}

bool ShaderManager::CreateInputLayout(const std::string & _Name, const std::string & _ShaderKey)
{
	ID3D11InputLayout* Layout = FindInputLayout(_Name);

	if (Layout != nullptr)
	{
		tassertmsg(true, "Create Overlapped InputLayout");
		return false;
	}

	Ezptr<AIMShader> Shader = FindShader(_ShaderKey);

	if (nullptr == Shader)
	{
		tassertmsg(true, "Unexist Shader");
		return false;
	}

	if (FAILED(GetAIMDevice->CreateInputLayout(&InputDescVec[0], (UINT)InputDescVec.size(), Shader->GetVSCode(), Shader->GetVsCodeSize(), &Layout)))
	{
		tassertmsg(true, "InputLayout Create Failed");
		return false;
	}

	InputDescVec.clear();
	InputSize = 0;

	InputLayoutMap.insert(std::unordered_map<std::string, ID3D11InputLayout*>::value_type(_Name, Layout));

	return true;
}

ID3D11InputLayout * ShaderManager::FindInputLayout(const std::string & _Name)
{
	std::unordered_map<std::string, ID3D11InputLayout*>::iterator FindIter = InputLayoutMap.find(_Name);

	if (FindIter == InputLayoutMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

bool ShaderManager::CreateConstBuffer(const std::string & _Name, int _Size, int _Register, int _ConstantShader)
{
	Ezptr<ConstBuffer> Buffer = FindConstBuffer(_Name);

	if (Buffer != nullptr)
	{
		tassertmsg(true, "Overlapped ConstBuffer Name");
		return false;
	}

	Buffer = new ConstBuffer;

	Buffer->Size = _Size;
	Buffer->Register = _Register;
	Buffer->Constant = _ConstantShader;
	Buffer->Name = _Name;

	Buffer->Data = new char[_Size];

	D3D11_BUFFER_DESC Desc = {};

	Desc.ByteWidth = _Size;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(GetAIMDevice->CreateBuffer(&Desc, nullptr, &Buffer->Buffer)))
	{
		delete Buffer->Data;
		tassertmsg(true, "Create ConstBuffer Failed");
		return false;
	}

	ConstBufferMap.insert(std::unordered_map<std::string, Ezptr<ConstBuffer>>::value_type(_Name, Buffer));

	return true;
}

bool ShaderManager::UpdateConstBuffer(const std::string & _Name, void * _Data)
{
	Ezptr<ConstBuffer> Buffer = FindConstBuffer(_Name);

	if (Buffer == nullptr)
	{
		tassertmsg(true, "Invalid ConstBuffer");
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE Map = {};

	GetAIMContext->Map(Buffer->Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

	memcpy(Map.pData, _Data, Buffer->Size);

	GetAIMContext->Unmap(Buffer->Buffer, 0);

	if (Buffer->Constant & CS_VERTEX)
	{
		GetAIMContext->VSSetConstantBuffers(Buffer->Register, 1, &Buffer->Buffer);
	}

	if (Buffer->Constant & CS_PIXEL)
	{
		GetAIMContext->PSSetConstantBuffers(Buffer->Register, 1, &Buffer->Buffer);
	}

	if (Buffer->Constant & CS_GEOMETRY)
	{
		GetAIMContext->GSSetConstantBuffers(Buffer->Register, 1, &Buffer->Buffer);
	}

	return true;
}

Ezptr<ConstBuffer> ShaderManager::FindConstBuffer(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<ConstBuffer>>::iterator FindIter = ConstBufferMap.find(_Name);

	if (FindIter == ConstBufferMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}