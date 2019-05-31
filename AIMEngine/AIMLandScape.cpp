#include "AIMLandScape.h"
#include "PathManager.h"
#include "AIMMesh.h"
#include "ResourceManager.h"
#include "AIMTexture.h"
#include "AIMMaterial.h"
#include "AIMRenderer.h"
#include "AIMObject.h"
#include "ShaderManager.h"
#include "NavigationManager.h"
#include "NavigationMesh.h"


AIMLandScape::AIMLandScape()
{
	CT = CT_LANDSCAPE;
	SetNameTag("LandScape");

	SetDetailLevel(30.0f, 30.0f);
	CBuffer.SplattingCount = 0;
}

AIMLandScape::AIMLandScape(const AIMLandScape & _Other) : AIMComponent(_Other)
{
	for (size_t i = 0; i < _Other.VtxVec.size(); i++)
	{
		VtxVec[i] = _Other.VtxVec[i];
	}

	for (size_t i = 0; i < _Other.IdxVec.size(); i++)
	{
		IdxVec[i] = _Other.IdxVec[i];
	}

	for (size_t i = 0; i < _Other.FaceNormal.size(); i++)
	{
		FaceNormal[i] = _Other.FaceNormal[i];
	}

	NumX = _Other.NumX;
	NumZ = _Other.NumZ;

	Material = _Other.Material;

	CBuffer = _Other.CBuffer;
	SplatName = _Other.SplatName;
	SpDifTex = _Other.SpDifTex;
	SpNrmTex = _Other.SpNrmTex;
	SpSpcTex = _Other.SpSpcTex;
	SpAlpTex = _Other.SpAlpTex;
}


AIMLandScape::~AIMLandScape()
{
	
}

bool AIMLandScape::CreateLandScape(const std::string & _Name, const char * _FileName, const std::string & _Path)
{
	char Ext[_MAX_EXT] = {};

	_splitpath_s(_FileName, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);

	_strupr_s(Ext);

	char FullPath[MAX_PATH] = {};

	const char* Path = PathManager::FindPathMultiByte(_Path);

	if (Path != nullptr)
	{
		strcpy_s(FullPath, Path);
	}

	strcat_s(FullPath, _FileName);

	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (File == nullptr)
	{
		return false;
	}

	if (strcmp(Ext, ".BMP") == 0)
	{
		BITMAPFILEHEADER FH;
		BITMAPINFOHEADER IH;

		fread(&FH, sizeof(FH), 1, File);
		fread(&IH, sizeof(IH), 1, File);

		int PixelLength = FH.bfSize - FH.bfOffBits;

		NumX = IH.biWidth;
		NumZ = IH.biHeight;

		unsigned char* Pixel = new unsigned char[PixelLength];

		fread(Pixel, 1, PixelLength, File);

		int PixelByte = 0;

		switch (IH.biBitCount)
		{
		case 8:
			PixelByte = 1;
			break;
		case 24:
			PixelByte = 3;
			break;
		case 32:
			PixelByte = 4;
			break;
		}

		unsigned char* Line = new unsigned char[NumX * PixelByte];

		for (int i = 0; i < NumZ / 2; i++)
		{
			int SrcIndex = i * NumX * PixelByte;
			int DestIndex = (NumZ - 1 - i) * NumX * PixelByte;

			memcpy(Line, &Pixel[SrcIndex], NumX * PixelByte);
			memcpy(&Pixel[SrcIndex], &Pixel[DestIndex], NumX * PixelByte);
			memcpy(&Pixel[DestIndex], Line, NumX * PixelByte);
		}

		delete[] Line;
		Line = nullptr;

		VtxVec.reserve(NumX * NumZ);

		for (int AA = 0; AA < NumZ; AA++)
		{
			for (int BB = 0; BB < NumX; BB++)
			{
				Vertex3D Vtx = {};
				int idx = AA * NumX * PixelByte + BB * PixelByte;

				float Y = Pixel[idx] / 20.0f;

				Vtx.Pos = Vec3((float)BB, Y, (float)(NumZ - AA - 1));
				Vtx.Normal = Vec3(0.0f, 0.0f, 0.0f);
				Vtx.UV = Vec2((float)BB / (NumX - 1.0f), (float)AA / (NumZ - 1.0f));
				//Vtx.UV = Vec2(AA, BB);
				Vtx.Tangent = Vec3(1.0f, 0.0f, 0.0f);
				Vtx.Binormal = Vec3(0.0f, 0.0f, -1.0f);

				VtxVec.push_back(Vtx);
			}
		}

		IdxVec.reserve((NumX - 1) * (NumZ - 1) * 2);
		FaceNormal.resize((NumX - 1) * (NumZ - 1) * 2);

		Ezptr<NavigationMesh> NaviMesh = NavigationManager::CreateNaviMesh(Scene, NameTag);

		int TriIndex = 0;
		Vec3 Edge1, Edge2;
		Vec3 CellPos[3];

		for (int AA = 0; AA < NumZ -1; AA++)
		{
			for (int BB = 0; BB < NumX - 1; BB++)
			{
				int idx = AA * NumX + BB;

				IdxVec.push_back(idx);
				IdxVec.push_back(idx + 1);
				IdxVec.push_back(idx + NumX + 1);

				CellPos[0] = VtxVec[idx].Pos;
				CellPos[1] = VtxVec[idx + 1].Pos;
				CellPos[2] = VtxVec[idx + NumX + 1].Pos;
				NaviMesh->AddCell(CellPos);

				Edge1 = VtxVec[idx + 1].Pos - VtxVec[idx].Pos;
				Edge2 = VtxVec[idx + NumX + 1].Pos - VtxVec[idx].Pos;

				Edge1.Normalize();
				Edge2.Normalize();

				FaceNormal[TriIndex] = Edge1.Cross(Edge2);
				FaceNormal[TriIndex].Normalize();
				++TriIndex;

				IdxVec.push_back(idx);
				IdxVec.push_back(idx + NumX + 1);
				IdxVec.push_back(idx + NumX);

				CellPos[0] = VtxVec[idx].Pos;
				CellPos[1] = VtxVec[idx + NumX + 1].Pos;
				CellPos[2] = VtxVec[idx + NumX].Pos;
				NaviMesh->AddCell(CellPos);

				Edge1 = VtxVec[idx + NumX + 1].Pos - VtxVec[idx].Pos;
				Edge2 = VtxVec[idx + NumX].Pos - VtxVec[idx].Pos;

				Edge1.Normalize();
				Edge2.Normalize();

				FaceNormal[TriIndex] = Edge1.Cross(Edge2);
				FaceNormal[TriIndex].Normalize();
				++TriIndex;
			}
		}

		delete[] Pixel;
		Pixel = nullptr;

		fclose(File);

		ComputeNormal();
		ComputeTangent();

		NaviMesh->CreateGridMapAdj(NumX - 1);

		ResourceManager::CreateMesh(_Name, "LandScapeShader", "StandardVertex3DLayout", Vec3(0.0f, 1.0f, 0.0f), sizeof(Vertex3D), VtxVec.size(), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &VtxVec[0], 4, IdxVec.size(), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, &IdxVec[0]);

		Ezptr<AIMRenderer> Renderer = FindComponent<AIMRenderer>(CT_RENDERER);

		if (Renderer == nullptr)
		{
			Renderer = Object->AddComponent<AIMRenderer>("LandScapeRenderer");
		}

		Renderer->SetMesh(_Name);

		Material = FindComponent<AIMMaterial>(CT_MATERIAL);

		if (Material == nullptr)
		{
			Material = Object->AddComponent<AIMMaterial>("LandScapeMaterial");
		}

		Material->AddTextureSet(0, 0, 0, "LandScapeDiffuse", TEXT("LandScape/BD_Terrain_Cave_01.dds"));
		Material->AddTextureSet(0, 0, 1, "LandScapeNormal", TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
		Material->AddTextureSet(0, 0, 2, "LandScapeSpecular", TEXT("LandScape/BD_Terrain_Cave_01_SPC.bmp"));

		Material->BumpTextureEnable(0, 0);
		Material->SpecularTextureEnable(0, 0);
	}

	fclose(File);

	return true;
}

void AIMLandScape::SetDetailLevel(float _LevelX, float _LevelZ)
{
	CBuffer.DetailLevelX = _LevelX;
	CBuffer.DetailLevelZ = _LevelZ;
}

void AIMLandScape::ChangeBaseTexture(const TCHAR * _DifName, const TCHAR * _NrmName, const TCHAR * _SpcName, const std::string & _Path)
{
	Material->ChangeTextureSet(0, 0, 0, "LandScapeDiffuse", "LandScapeDiffuse", _DifName, _Path);
	Material->ChangeTextureSet(0, 0, 1, "LandScapeNormal", "LandScapeNormal", _NrmName, _Path);
	Material->ChangeTextureSet(0, 0, 2, "LandScapeSpecular", "LandScapeSpecular", _SpcName, _Path);
}

void AIMLandScape::AddSplattingTexture(const TCHAR * _DifName, const TCHAR * _NrmName, const TCHAR * _SpcName, const TCHAR * _AlpName, const std::string & _Path)
{
	TCHAR* Name = new TCHAR[MAX_PATH];
	memset(Name, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(Name, _DifName);
	SplatName.DifName.push_back(Name);

	Name = new TCHAR[MAX_PATH];
	memset(Name, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(Name, _NrmName);
	SplatName.NrmName.push_back(Name);

	Name = new TCHAR[MAX_PATH];
	memset(Name, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(Name, _SpcName);
	SplatName.SpcName.push_back(Name);

	Name = new TCHAR[MAX_PATH];
	memset(Name, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(Name, _AlpName);
	SplatName.AlpName.push_back(Name);

	SplatName.PathName.push_back(_Path);
}

void AIMLandScape::CreateSplattingTexture()
{
	if (SplatName.DifName.empty() == true)
	{
		return;
	}

	SpDifTex = nullptr;
	SpNrmTex = nullptr;
	SpSpcTex = nullptr;
	SpAlpTex = nullptr;

	Material->ChangeTextureSetArray(0, 0, 10, "SplattingDiffuse", "SplattingDiffuse", &SplatName.DifName, SplatName.PathName[0]);
	Material->ChangeTextureSetArray(0, 0, 11, "SplattingNormal", "SplattingNormal", &SplatName.NrmName, SplatName.PathName[0]);
	Material->ChangeTextureSetArray(0, 0, 12, "SplattingSpecular", "SplattingSpecular", &SplatName.SpcName, SplatName.PathName[0]);
	Material->ChangeTextureSetArray(0, 0, 13, "SplattingAlpha", "SplattingAlpha", &SplatName.AlpName, SplatName.PathName[0]);

	CBuffer.SplattingCount = SplatName.DifName.size();
}

void AIMLandScape::ComputeNormal()
{
	for (size_t i = 0; i < FaceNormal.size(); ++i)
	{
		int	idx0 = IdxVec[i * 3];
		int	idx1 = IdxVec[i * 3 + 1];
		int	idx2 = IdxVec[i * 3 + 2];

		VtxVec[idx0].Normal += FaceNormal[i];
		VtxVec[idx1].Normal += FaceNormal[i];
		VtxVec[idx2].Normal += FaceNormal[i];
	}

	for (size_t i = 0; i < VtxVec.size(); ++i)
	{
		VtxVec[i].Normal.Normalize();
	}
}

void AIMLandScape::ComputeTangent()
{
	for (size_t i = 0; i < FaceNormal.size(); ++i)
	{
		int	idx0 = IdxVec[i * 3];
		int	idx1 = IdxVec[i * 3 + 1];
		int	idx2 = IdxVec[i * 3 + 2];

		float	fVtx1[3], fVtx2[3];
		fVtx1[0] = VtxVec[idx1].Pos.x - VtxVec[idx0].Pos.x;
		fVtx1[1] = VtxVec[idx1].Pos.y - VtxVec[idx0].Pos.y;
		fVtx1[2] = VtxVec[idx1].Pos.z - VtxVec[idx0].Pos.z;

		fVtx2[0] = VtxVec[idx2].Pos.x - VtxVec[idx0].Pos.x;
		fVtx2[1] = VtxVec[idx2].Pos.y - VtxVec[idx0].Pos.y;
		fVtx2[2] = VtxVec[idx2].Pos.z - VtxVec[idx0].Pos.z;

		float	ftu[2], ftv[2];
		ftu[0] = VtxVec[idx1].UV.x - VtxVec[idx0].UV.x;
		ftv[0] = VtxVec[idx1].UV.y - VtxVec[idx0].UV.y;

		ftu[1] = VtxVec[idx2].UV.x - VtxVec[idx0].UV.x;
		ftv[1] = VtxVec[idx2].UV.y - VtxVec[idx0].UV.y;

		float	fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

		Vec3	vTangent;
		vTangent.x = (ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0]) * fDen;
		vTangent.y = (ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1]) * fDen;
		vTangent.z = (ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2]) * fDen;

		vTangent.Normalize();

		VtxVec[idx0].Tangent = vTangent;
		VtxVec[idx1].Tangent = vTangent;
		VtxVec[idx2].Tangent = vTangent;

		VtxVec[idx0].Binormal = VtxVec[idx0].Normal.Cross(vTangent);
		VtxVec[idx1].Binormal = VtxVec[idx1].Normal.Cross(vTangent);
		VtxVec[idx2].Binormal = VtxVec[idx2].Normal.Cross(vTangent);

		VtxVec[idx0].Binormal.Normalize();
		VtxVec[idx1].Binormal.Normalize();
		VtxVec[idx2].Binormal.Normalize();
	}
}

void AIMLandScape::Start()
{
	Material = FindComponent<AIMMaterial>(CT_MATERIAL);

	Object->EraseComponent("PickSphere");
}

bool AIMLandScape::Init()
{
	return true;
}

int AIMLandScape::Input(float _Time)
{
	return 0;
}

int AIMLandScape::Update(float _Time)
{
	return 0;
}

int AIMLandScape::LateUpdate(float _Time)
{
	return 0;
}

int AIMLandScape::Collision(float _Time)
{
	return 0;
}

int AIMLandScape::PrevRender(float _Time)
{
	ShaderManager::UpdateConstBuffer("LandScape", &CBuffer);

	return 0;
}

int AIMLandScape::Render(float _Time)
{
	return 0;
}

AIMLandScape * AIMLandScape::Clone() const
{
	return new AIMLandScape(*this);
}
