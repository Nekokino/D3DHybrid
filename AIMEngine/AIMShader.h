#pragma once

#include "RefCounter.h"
class Engine_DLL AIMShader : public RefCounter
{
public:
	friend class ShaderManager;

private:
	ID3D11VertexShader* VS;
	ID3DBlob* VSBlob;
	ID3D11PixelShader* PS;
	ID3DBlob* PSBlob;

public:
	const void* GetVSCode() const;
	size_t GetVsCodeSize() const;

public:
	bool LoadShader(const std::string& _Name, const TCHAR* _FileName, std::string _Entry[ST_END], const std::string& _PathKey);
	void SetShader();

private:
	bool LoadVertexShader(const TCHAR* _FullPath, const char* _Entry);
	bool LoadPixelShader(const TCHAR* _FullPath, const char* _Entry);

private:
	AIMShader();
	~AIMShader();
};

