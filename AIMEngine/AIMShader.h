#pragma once

#include "RefCounter.h"
class Engine_DLL AIMShader : public RefCounter
{
public:
	friend class ShaderManager;

private:
	ID3D11VertexShader* VS = nullptr;
	ID3DBlob* VSBlob = nullptr;
	ID3D11GeometryShader* GS = nullptr;
	ID3DBlob* GSBlob = nullptr;
	ID3D11PixelShader* PS = nullptr;
	ID3DBlob* PSBlob = nullptr;

public:
	const void* GetVSCode() const;
	size_t GetVsCodeSize() const;

public:
	bool LoadShader(const std::string& _Name, const TCHAR* _FileName, std::string _Entry[ST_END], const std::string& _PathKey);
	void SetShader();

private:
	bool LoadVertexShader(const TCHAR* _FullPath, const char* _Entry);
	bool LoadPixelShader(const TCHAR* _FullPath, const char* _Entry);
	bool LoadGeometryShader(const TCHAR* _FullPath, const char* _Entry);

private:
	AIMShader();
	~AIMShader();
};

