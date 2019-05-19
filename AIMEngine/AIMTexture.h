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
	std::vector<TCHAR*> FullPathVec;

public:
	const std::vector<TCHAR*>* GetFullPath() const
	{
		return &FullPathVec;
	}

public:
	// 파일 이름과 미리 지정된 경로를 통해 로드 텍스처
	bool LoadTexture(const std::string& _Name, const TCHAR* _FileName, const std::string& _Path);
	// 전체 경로에서 로드 텍스처
	bool LoadTexture(const std::string& _Name, const TCHAR* _FullPath);

	bool LoadTexture(const std::string& _Name, const std::vector<TCHAR*>& _FileNameVec, const std::string& _Path);
	bool LoadTexture(const std::string& _Name, const std::vector<TCHAR*>& _FullPathVec);

	void VSSetShader(int _Register);
	void PSSetShader(int _Register);

private:
	bool CreateShaderResourceView();
	bool CreateShaderResourceViewArray();

private:
	AIMTexture();
	~AIMTexture();
};

