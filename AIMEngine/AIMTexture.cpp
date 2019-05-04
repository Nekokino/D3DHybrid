#include "AIMTexture.h"
#include "PathManager.h"
#include "AIMDevice.h"

AIMTexture::AIMTexture()
{
}


AIMTexture::~AIMTexture()
{
	for (size_t i = 0; i < ImageVec.size(); ++i)
	{
		if (ImageVec[i] != nullptr)
		{
			delete ImageVec[i];
			ImageVec[i] = nullptr;
		}
	}

	if (SRV != nullptr)
	{
		SRV->Release();
		SRV = nullptr;
	}
}

bool AIMTexture::LoadTexture(const std::string _Name, const TCHAR * _FileName, const std::string & _Path)
{
	const TCHAR* Path = PathManager::FindPath(_Path);

	TCHAR FullPath[MAX_PATH] = {};

	if (Path != nullptr)
	{
		lstrcpy(FullPath, Path);
	}

	lstrcat(FullPath, _FileName);

	return LoadTexture(_Name, FullPath);
}

#ifdef UNICODE
	#define SplitPath _wsplitpath_s
#else
	#define SplitPath _splitpath_s
#endif


bool AIMTexture::LoadTexture(const std::string _Name, const TCHAR * _FullPath)
{
	SetNameTag(_Name.c_str());

	DirectX::ScratchImage* Image = new DirectX::ScratchImage;

	TCHAR Ext[_MAX_EXT] = {};

	SplitPath(_FullPath, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);

	char Ext2[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, Ext, -1, Ext2, lstrlen(Ext), 0, 0);

	_strupr_s(Ext2);

	if (strcmp(Ext2, ".DDS") == 0)
	{
		if (FAILED(DirectX::LoadFromDDSFile(_FullPath, DirectX::DDS_FLAGS_NONE, nullptr, *Image)))
		{
			tassertmsg(true, "DDS File Load Failed");
			return false;
		}
	}

	else if (strcmp(Ext2, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(_FullPath, nullptr, *Image)))
		{
			tassertmsg(true, "TAG File Load Failed");
			return false;
		}
	}

	else
	{
		if (FAILED(DirectX::LoadFromWICFile(_FullPath, DirectX::WIC_FLAGS_NONE, nullptr, *Image)))
		{
			tassertmsg(true, "WIC File Load Failed");
			return false;
		}
	}

	ImageVec.push_back(Image);

	return CreateShaderResourceView();
}

void AIMTexture::VSSetShader(int _Register)
{
	GetAIMContext->VSSetShaderResources(_Register, 1, &SRV);
}

void AIMTexture::PSSetShader(int _Register)
{
	GetAIMContext->PSSetShaderResources(_Register, 1, &SRV);
}

bool AIMTexture::CreateShaderResourceView()
{
	if (FAILED(DirectX::CreateShaderResourceView(GetAIMDevice, ImageVec[0]->GetImages(), ImageVec[0]->GetImageCount(), ImageVec[0]->GetMetadata(), &SRV)))
	{
		tassertmsg(true, "Create Shader Resource View Failed");
		return false;
	}

	return true;
}
