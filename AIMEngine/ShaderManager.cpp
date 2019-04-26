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
	char* Entry[ST_END] = {};

	Entry[ST_VTX] = "StandardColorVS";
	Entry[ST_PIX] = "StandardColorPS";

	if (false == LoadShader("StandardColorShader", TEXT("Standard.fx"), Entry, "Shader"))
	{
		return false;
	}

	// �Է� ���̾ƿ� ���� ����
	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (false == CreateInputLayout("StandardColorLayout", "StandardColorShader"))
	{
		return false;
	}

	CreateConstBuffer("Transform", sizeof(TransformConstBuffer), 0, CS_VERTEX | CS_PIXEL);


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
			//Release �����൵ ������ ����? ��� ���쳪?
			//StartIter->second->Release();
			StartIter->second = nullptr;
		}
	}

	InputLayoutMap.clear();
	ConstBufferMap.clear();
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

	// �� ��ǲ ������� ���� ��ǲ ��ġ�� �������ֱ� ���ؼ� �������� ������ش�.
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