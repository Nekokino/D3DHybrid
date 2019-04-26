#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"
#include "RefStruct.h"

class AIMShader;
class Engine_DLL ShaderManager
{
private:
	static std::unordered_map<std::string, Ezptr<AIMShader>> ShaderMap;
	static std::unordered_map<std::string, ID3D11InputLayout*> InputLayoutMap;
	static std::unordered_map<std::string, Ezptr<ConstBuffer>> ConstBufferMap;

	static std::vector<D3D11_INPUT_ELEMENT_DESC> InputDescVec;
	static UINT InputSize;

public:
	static bool Init();

	// (���̴� �̸�, ���̴� ���� �̸�, , ��� �̸�)
	static bool LoadShader(const std::string& _Name, const TCHAR* _FileName, char* _Entry[ST_END], const std::string& _PathKey);

	static Ezptr<AIMShader> FindShader(const std::string& _Name);

	// �� ��ǲ�����͵��� �ϳ��� �߰����ش�.
	// �ø�ƽ �̸�, �ø�ƽ �ε���, �ø�ƽ ����, ������, ��ǲ����, ����
	static void AddInputDesc(const char* _Semantic, UINT _SemanticIdx, DXGI_FORMAT _Format, UINT _Size, UINT _InputSlot, D3D11_INPUT_CLASSIFICATION _Class, UINT _StepRate);
	// AddInputDesc�� �߰����� �����͵��� �����Ͽ� ��ǲ ���̾ƿ��� �����Ѵ�.
	// ���̾ƿ� �̸�, ������ ���̴�
	static bool CreateInputLayout(const std::string& _Name, const std::string& _ShaderKey);
	static ID3D11InputLayout* FindInputLayout(const std::string& _Name);

	static bool CreateConstBuffer(const std::string& _Name, int _Size, int _Register, int _ConstantShader);
	static bool UpdateConstBuffer(const std::string& _Name, void* _Data);
	static Ezptr<ConstBuffer> FindConstBuffer(const std::string& _Name);

	static void Release();
public:
	ShaderManager() {}
	virtual ~ShaderManager() = 0 {}
};

