#pragma once
#include "RefCounter.h"
#include "DirectXTex.h"

#ifdef _DEBUG
	#ifdef _M_IX86
		#pragma comment(lib, "DirectXTex_D86")
	#else
		#pragma comment(lib, "DirectXTex_D64")
	#endif
#else
	#ifdef _M_IX86
		#pragma comment(lib, "DirectXTex_R86")
	#else
		#pragma comment(lib, "DirectXTex_R64")
	#endif
#endif

class Engine_DLL AIMTexture : public RefCounter
{
	friend class ResourceManager;
private:
	std::vector<DirectX::ScratchImage*> ImageVec;
	ID3D11ShaderResourceView* SRV = nullptr;

public:
	// ���� �̸��� �̸� ������ ��θ� ���� �ε� �ؽ�ó
	bool LoadTexture(const std::string _Name, const TCHAR* _FileName, const std::string& _Path);
	// ��ü ��ο��� �ε� �ؽ�ó
	bool LoadTexture(const std::string _Name, const TCHAR* _FullPath);

	void VSSetShader(int _Register);
	void PSSetShader(int _Register);

private:
	bool CreateShaderResourceView();

private:
	AIMTexture();
	~AIMTexture();
};

