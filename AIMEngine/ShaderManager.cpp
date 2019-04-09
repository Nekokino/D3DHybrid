#include "ShaderManager.h"
#include "AIMShader.h"
#include "AIMDevice.h"

std::unordered_map<std::string, Ezptr<AIMShader>> ShaderManager::ShaderMap;
std::unordered_map<std::string, ID3D11InputLayout*> ShaderManager::InputLayoutMap;

std::vector<D3D11_INPUT_ELEMENT_DESC> ShaderManager::InputDescVec;
UINT ShaderManager::InputSize = 0;

bool ShaderManager::Init()
{
	char* Entry[ST_END] = {};

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

	return true;
}

bool ShaderManager::LoadShader(const std::string & _Name, const TCHAR * _FileName, char * _Entry[ST_END], const std::string & _PathKey)
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

	if (FAILED(GetAIMDevice->CreateInputLayout(&InputDescVec[0], InputDescVec.size(), Shader->GetVSCode(), Shader->GetVsCodeSize(), &Layout)))
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
}
