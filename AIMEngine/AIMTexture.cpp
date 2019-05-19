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

	for (size_t i = 0; i < FullPathVec.size(); i++)
	{
		if (FullPathVec[i] != nullptr)
		{
			delete[] FullPathVec[i];
			FullPathVec[i] = nullptr;
		}
	}

	FullPathVec.clear();
}

bool AIMTexture::LoadTexture(const std::string& _Name, const TCHAR * _FileName, const std::string & _Path)
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


bool AIMTexture::LoadTexture(const std::string& _Name, const TCHAR * _FullPath)
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

	TCHAR* SavePath = new TCHAR[MAX_PATH];

	memset(SavePath, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(SavePath, _FullPath);

	FullPathVec.push_back(SavePath);

	ImageVec.push_back(Image);

	return CreateShaderResourceView();
}

bool AIMTexture::LoadTexture(const std::string & _Name, const std::vector<TCHAR*>& _FileNameVec, const std::string & _Path)
{
	const TCHAR* Path = PathManager::FindPath(_Path);

	std::vector<TCHAR*> FullPathVec;

	FullPathVec.reserve(_FileNameVec.size());

	for (size_t i = 0; i < _FileNameVec.size(); i++)
	{
		TCHAR* FullPath = new TCHAR[MAX_PATH];
		memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

		if (Path != nullptr)
		{
			lstrcpy(FullPath, Path);
		}

		lstrcat(FullPath, _FileNameVec[i]);

		FullPathVec.push_back(FullPath);
	}

	if (false == LoadTexture(_Name, FullPathVec))
	{
		for (size_t i = 0; i < FullPathVec.size(); i++)
		{
			if (FullPathVec[i] != nullptr)
			{
				delete[] FullPathVec[i];
				FullPathVec[i] = nullptr;
			}
		}
		return false;
	}

	for (size_t i = 0; i < FullPathVec.size(); i++)
	{
		if (FullPathVec[i] != nullptr)
		{
			delete[] FullPathVec[i];
			FullPathVec[i] = nullptr;
		}
	}

	return true;

}

bool AIMTexture::LoadTexture(const std::string & _Name, const std::vector<TCHAR*>& _FullPathVec)
{
	SetNameTag(_Name.c_str());

	for (size_t i = 0; i < _FullPathVec.size(); i++)
	{
		DirectX::ScratchImage* Image = new DirectX::ScratchImage;

		TCHAR Ext[_MAX_EXT] = {};

		SplitPath(_FullPathVec[i], nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);

		char Ext2[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, Ext, -1, Ext2, lstrlen(Ext), 0, 0);

		_strupr_s(Ext2);

		if (strcmp(Ext2, ".DDS") == 0)
		{
			if (FAILED(DirectX::LoadFromDDSFile(_FullPathVec[i], DirectX::DDS_FLAGS_NONE, nullptr, *Image)))
			{
				tassertmsg(true, "DDS File Load Failed");
				return false;
			}
		}

		else if (strcmp(Ext2, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(_FullPathVec[i], nullptr, *Image)))
			{
				tassertmsg(true, "TAG File Load Failed");
				return false;
			}
		}

		else
		{
			if (FAILED(DirectX::LoadFromWICFile(_FullPathVec[i], DirectX::WIC_FLAGS_NONE, nullptr, *Image)))
			{
				tassertmsg(true, "WIC File Load Failed");
				return false;
			}
		}

		TCHAR* SavePath = new TCHAR[MAX_PATH];

		memset(SavePath, 0, sizeof(TCHAR) * MAX_PATH);

		lstrcpy(SavePath, _FullPathVec[i]);

		FullPathVec.push_back(SavePath);

		ImageVec.push_back(Image);
	}

	return CreateShaderResourceViewArray();
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

bool AIMTexture::CreateShaderResourceViewArray()
{
	std::vector<ID3D11Texture2D*> TextureVec;

	for (size_t i = 0; i < ImageVec.size(); i++)
	{
		ID3D11Texture2D* Tex = NULL;

		if (FAILED(DirectX::CreateTextureEx(GetAIMDevice, ImageVec[i]->GetImages(), ImageVec[i]->GetImageCount(), ImageVec[i]->GetMetadata(), D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ, 0, FALSE, (ID3D11Resource**)&Tex)))
		{
			tassertmsg(true, "Failed Create Staging Textrue");
			return false;
		}

		TextureVec.push_back(Tex);
	}

	D3D11_TEXTURE2D_DESC TexDesc = {};

	TextureVec[0]->GetDesc(&TexDesc);

	D3D11_TEXTURE2D_DESC	ArrayDesc = {};
	ArrayDesc.Width = TexDesc.Width;
	ArrayDesc.Height = TexDesc.Height;
	ArrayDesc.MipLevels = TexDesc.MipLevels;
	ArrayDesc.ArraySize = TextureVec.size();
	ArrayDesc.Format = TexDesc.Format;
	ArrayDesc.SampleDesc.Count = 1;
	ArrayDesc.SampleDesc.Quality = 0;
	ArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	ArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* ArrayTex = NULL;
	if (FAILED(GetAIMDevice->CreateTexture2D(&ArrayDesc, NULL, &ArrayTex)))
	{
		tassertmsg(true, "Failed to Craete Texture2D");
		return false;
	}

	for (size_t AA = 0; AA < TextureVec.size(); AA++)
	{
		for (size_t mip = 0; mip < TexDesc.MipLevels; mip++)
		{
			D3D11_MAPPED_SUBRESOURCE Map = {};
			GetAIMContext->Map(TextureVec[AA], mip, D3D11_MAP_READ, 0, &Map);

			GetAIMContext->UpdateSubresource(ArrayTex, D3D11CalcSubresource(mip, AA, TexDesc.MipLevels), NULL, Map.pData, Map.RowPitch, Map.DepthPitch);

			GetAIMContext->Unmap(TextureVec[AA], mip);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC ViewDesc = {};
	ViewDesc.Format = ArrayDesc.Format;
	ViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	ViewDesc.Texture2DArray.MostDetailedMip = 0;
	ViewDesc.Texture2DArray.MipLevels = ArrayDesc.MipLevels;
	ViewDesc.Texture2DArray.FirstArraySlice = 0;
	ViewDesc.Texture2DArray.ArraySize = TextureVec.size();

	if (FAILED(GetAIMDevice->CreateShaderResourceView(ArrayTex, &ViewDesc, &SRV)))
	{
		tassertmsg(true, "Failed To Create ShaderResourceView");
		return false;
	}

	if (ArrayTex != nullptr)
	{
		ArrayTex->Release();
		ArrayTex = nullptr;
	}

	for (size_t i = 0; i < TextureVec.size(); i++)
	{
		if (TextureVec[i] != nullptr)
		{
			TextureVec[i]->Release();
			TextureVec[i] = nullptr;
		}
	}

	return true;
}
