#include "AIMShader.h"
#include "PathManager.h"
#include "AIMDevice.h"

AIMShader::AIMShader()
{
}


AIMShader::~AIMShader()
{
	if (nullptr != VS)
	{
		VS->Release();
		VS = nullptr;
	}

	if (nullptr != VSBlob)
	{
		VSBlob->Release();
		VSBlob = nullptr;
	}

	if (nullptr != PS)
	{
		PS->Release();
		PS = nullptr;
	}

	if (nullptr != PSBlob)
	{
		PSBlob->Release();
		PSBlob = nullptr;
	}

	if (nullptr != GS)
	{
		GS->Release();
		GS = nullptr;
	}

	if (nullptr != GSBlob)
	{
		GSBlob->Release();
		GSBlob = nullptr;
	}
}


const void * AIMShader::GetVSCode() const
{
	return VSBlob->GetBufferPointer();
}

size_t AIMShader::GetVsCodeSize() const
{
	return VSBlob->GetBufferSize();
}

bool AIMShader::LoadShader(const std::string & _Name, const TCHAR * _FileName, std::string _Entry[ST_END], const std::string & _PathKey)
{
	SetNameTag(_Name.c_str());

	const TCHAR* FindPath = PathManager::FindPath(_PathKey);

	TCHAR Path[MAX_PATH] = {};

	if (nullptr != FindPath)
	{
		lstrcpy(Path, FindPath);
	}

	lstrcat(Path, _FileName);

	if (false == LoadVertexShader(Path, _Entry[ST_VTX].c_str()))
	{
		tassertmsg(true, "VertexShader Load Failed");
		return false;
	}

	if (false == LoadPixelShader(Path, _Entry[ST_PIX].c_str()))
	{
		tassertmsg(true, "PixelShader Load Failed");
		return false;
	}

	if (_Entry[ST_GEO].empty() == false)
	{
		if (false == LoadGeometryShader(Path, _Entry[ST_GEO].c_str()))
		{
			tassertmsg(true, "GeometryShader Load Failed");
			return false;
		}
	}
	

	return true;
}

void AIMShader::SetShader()
{
	GetAIMContext->VSSetShader(VS, nullptr, 0);
	GetAIMContext->PSSetShader(PS, nullptr, 0);
	GetAIMContext->GSSetShader(GS, nullptr, 0);
}

bool AIMShader::LoadVertexShader(const TCHAR * _FullPath, const char * _Entry)
{
	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* Err = nullptr;

	if (FAILED(D3DCompileFromFile(_FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _Entry, "vs_5_0", Flag, 0, &VSBlob, &Err)))
	{
		OutputDebugStringA((char*)Err->GetBufferPointer());
		tassertmsg(true, "Vertex Shader Compile Error");
		return false;
	}

	if (FAILED(GetAIMDevice->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &VS)))
	{
		tassertmsg(true, "Create Vertex Shader Error");
		return false;
	}

	return true;
}

bool AIMShader::LoadPixelShader(const TCHAR * _FullPath, const char * _Entry)
{
	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* Err = nullptr;

	if (FAILED(D3DCompileFromFile(_FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _Entry, "ps_5_0", Flag, 0, &PSBlob, &Err)))
	{
		OutputDebugStringA((char*)Err->GetBufferPointer());
		tassertmsg(true, "Pixel Shader Compile Error");
		return false;
	}

	if (FAILED(GetAIMDevice->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &PS)))
	{
		tassertmsg(true, "Create Pixel Shader Error");
		return false;
	}

	return true;
}

bool AIMShader::LoadGeometryShader(const TCHAR * _FullPath, const char * _Entry)
{
	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* Err = nullptr;

	if (FAILED(D3DCompileFromFile(_FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _Entry, "gs_5_0", Flag, 0, &GSBlob, &Err)))
	{
		OutputDebugStringA((char*)Err->GetBufferPointer());
		tassertmsg(true, "Geometry Shader Compile Error");
		return false;
	}

	if (FAILED(GetAIMDevice->CreateGeometryShader(GSBlob->GetBufferPointer(), GSBlob->GetBufferSize(), nullptr, &GS)))
	{
		tassertmsg(true, "Create Geometry Shader Error");
		return false;
	}

	return true;
}

