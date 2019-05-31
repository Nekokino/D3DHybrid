#include "AIMMesh.h"
#include "AIMDevice.h"
#include "PathManager.h"
#include "FbxLoader.h"
#include "AIMMaterial.h"
#include "AIMAnimation.h"

AIMMesh::AIMMesh()
{
	View = Vec3::Axis[AXIS_Z];
}


AIMMesh::~AIMMesh()
{
	for (size_t AA = 0; AA < MeshContainerVec.size(); ++AA)
	{
		for (size_t BB = 0; BB < MeshContainerVec[AA]->IBVec.size(); ++BB)
		{
			if (nullptr != MeshContainerVec[AA]->IBVec[BB]->Data)
			{
				delete MeshContainerVec[AA]->IBVec[BB]->Data;
				MeshContainerVec[AA]->IBVec[BB]->Data = nullptr;
			}

			if (nullptr != MeshContainerVec[AA]->IBVec[BB]->Buffer)
			{
				MeshContainerVec[AA]->IBVec[BB]->Buffer->Release();
				MeshContainerVec[AA]->IBVec[BB]->Buffer = nullptr;
			}

			if (nullptr != MeshContainerVec[AA]->IBVec[BB])
			{
				delete MeshContainerVec[AA]->IBVec[BB];
				MeshContainerVec[AA]->IBVec[BB] = nullptr;
			}
		}

		if (nullptr != MeshContainerVec[AA]->VB.Data)
		{
			delete MeshContainerVec[AA]->VB.Data;
			MeshContainerVec[AA]->VB.Data = nullptr;
		}

		if (nullptr != MeshContainerVec[AA]->VB.Buffer)
		{
			MeshContainerVec[AA]->VB.Buffer->Release();
			MeshContainerVec[AA]->VB.Buffer = nullptr;
		}

		if (nullptr != MeshContainerVec[AA])
		{
			delete MeshContainerVec[AA];
			MeshContainerVec[AA] = nullptr;
		}
	}

	MeshContainerVec.clear();
}


bool AIMMesh::CreateMesh(const std::string & _Name, const std::string& _ShaderName, const std::string& _InputLayoutName, int _VtxSize, int _VtxCount, D3D11_USAGE _VtxUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void * _Vtx, int _IdxSize, int _IdxCount, D3D11_USAGE _IdxUsage, DXGI_FORMAT _Format, void * _Idx)
{
	NameTag = _Name;
	ShaderName = _ShaderName;
	InputLayoutName = _InputLayoutName;

	MeshContainer* Container = new MeshContainer;

	MeshContainerVec.push_back(Container);

	if (false == CreateVertexBuffer(_VtxSize, _VtxCount, _VtxUsage, _Primitive, _Vtx))
	{
		return false;
	}

	if (_Idx != nullptr)
	{
		if (false == CreateIndexBuffer(_IdxSize, _IdxCount, _IdxUsage, _Format, _Idx))
		{
			return false;
		}
	}

	return true;
}

bool AIMMesh::LoadMesh(const std::string & _Name, const TCHAR * _FileName, const Vec3 & _View, const std::string & _Path)
{
	const TCHAR* Path = PathManager::FindPath(_Path);

	TCHAR FullPath[MAX_PATH] = {};

	if (Path != nullptr)
	{
		lstrcpy(FullPath, Path);
	}

	lstrcat(FullPath, _FileName);

	return LoadMeshFromFullPath(_Name, FullPath);
}

bool AIMMesh::LoadMesh(const std::string & _Name, const char * _FileName, const Vec3 & _View, const std::string & _Path)
{
	const char* Path = PathManager::FindPathMultiByte(_Path);

	char FullPath[MAX_PATH] = {};

	if (Path != nullptr)
	{
		strcpy_s(FullPath, Path);
	}

	strcat_s(FullPath, _FileName);

	return LoadMeshFromFullPath(_Name, FullPath);
}

bool AIMMesh::LoadMeshFromFullPath(const std::string & _Name, const TCHAR * _FullPath, const Vec3 & _View)
{
	char FullPath[MAX_PATH] = {};

	WideCharToMultiByte(CP_UTF8, 0, _FullPath, -1, FullPath, lstrlen(_FullPath), 0, 0);

	return LoadMeshFromFullPath(_Name, FullPath);
}

bool AIMMesh::LoadMeshFromFullPath(const std::string & _Name, const char * _FullPath, const Vec3 & _View)
{
	SetNameTag(_Name);

	View = _View;

	char Ext[_MAX_EXT] = {};

	_splitpath_s(_FullPath, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);

	_strupr_s(Ext);

	if (strcmp(Ext, ".FBX") == 0)
	{
		FbxLoader Loader;
		
		Loader.LoadFbx(_FullPath);

		return ConvertFbx(&Loader, _FullPath);
	}

	return LoadFullPath(_FullPath);
}

void AIMMesh::Render()
{
	for (size_t AA = 0; AA < MeshContainerVec.size(); ++AA)
	{
		MeshContainer* Container = MeshContainerVec[AA];

		UINT Stride = Container->VB.Size;
		UINT Offset = 0;

		// 컨텍스트에 버텍스 버퍼의 토폴로지 가져와서 세팅
		GetAIMContext->IASetPrimitiveTopology(Container->VB.Primitive);
		// 컨텍스트에 버텍스 버퍼 세팅
		// (앞의 첫번째 인자는 어차피 버텍스버퍼를 한개만 사용할거라 0 고정이고, 두번째 인자 역시 1개만 사용할 것이므로 1 고정이다)
		GetAIMContext->IASetVertexBuffers(0, 1, &Container->VB.Buffer, &Stride, &Offset);

		if (true == Container->IBVec.empty())
		{
			// 인덱스 버퍼가 없다면 그냥 깡으로 그리고
			GetAIMContext->Draw(Container->VB.Count, 0);
		}
		else
		{
			for (size_t BB = 0; BB < Container->IBVec.size(); ++BB)
			{
				IndexBuffer* Subset = Container->IBVec[BB];

				// 인덱스버퍼가 있다면 인덱스 버퍼를 컨텍스트에 세팅하고
				GetAIMContext->IASetIndexBuffer(Subset->Buffer, Subset->Format, 0);
				// 인덱스 버퍼대로 그려준다.
				GetAIMContext->DrawIndexed(Subset->Count, 0, 0);
			}
		}
	}
}

void AIMMesh::Render(size_t _Container, size_t _SubSet)
{
	MeshContainer* Container = MeshContainerVec[_Container];

	UINT Stride = Container->VB.Size;
	UINT Offset = 0;

	GetAIMContext->IASetPrimitiveTopology(Container->VB.Primitive);
	GetAIMContext->IASetVertexBuffers(0, 1, &Container->VB.Buffer, &Stride, &Offset);

	if (true == Container->IBVec.empty())
	{
		GetAIMContext->Draw(Container->VB.Count, 0);
	}
	else
	{
		IndexBuffer* Subset = Container->IBVec[_SubSet];

		GetAIMContext->IASetIndexBuffer(Subset->Buffer, Subset->Format, 0);
		GetAIMContext->DrawIndexed(Subset->Count, 0, 0);
	}
}

void AIMMesh::RenderInstancing(unsigned int _Container, unsigned int _Subset, InstancingBuffer* _InstancingBuffer, int _InstancingCount)
{
	MeshContainer* Container = MeshContainerVec[_Container];

	UINT Stride[2] = { Container->VB.Size, _InstancingBuffer->Size };
	UINT Offset[2] = { 0, 0 };

	ID3D11Buffer* Buffer[2] = { Container->VB.Buffer, _InstancingBuffer->Buffer };

	GetAIMContext->IASetPrimitiveTopology(Container->VB.Primitive);
	GetAIMContext->IASetVertexBuffers(0, 2, Buffer, Stride, Offset);

	if (Container->IBVec.empty() == true)
	{
		GetAIMContext->DrawInstanced(Container->VB.Count, _InstancingCount, 0, 0);
	}
	else
	{
		IndexBuffer* Subset = Container->IBVec[_Subset];

		GetAIMContext->IASetIndexBuffer(Subset->Buffer, Subset->Format, 0);
		GetAIMContext->DrawIndexedInstanced(Subset->Count, _InstancingCount, 0, 0, 0);
	}
}

bool AIMMesh::CreateVertexBuffer(int _Size, int _Count, D3D11_USAGE _Usage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void * _Data)
{
	MeshContainer* Container = MeshContainerVec.back();

	D3D11_BUFFER_DESC Desc = {};

	Desc.ByteWidth = _Size * _Count;
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Desc.Usage = _Usage;

	switch (_Usage)
	{
	case D3D11_USAGE_DYNAMIC:
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	Container->VB.Size = _Size;
	Container->VB.Count = _Count;
	Container->VB.Usage = _Usage;
	Container->VB.Primitive = _Primitive;
	Container->VB.Data = new char[_Size * _Count];

	memcpy(Container->VB.Data, _Data, _Size * _Count);

	D3D11_SUBRESOURCE_DATA Data = {};

	Data.pSysMem = Container->VB.Data;

	if (FAILED(GetAIMDevice->CreateBuffer(&Desc, &Data, &Container->VB.Buffer)))
	{
		tassertmsg(true, "VertexBuffer Create Failed");
		return false;
	}

	char* Vertices = (char*)_Data;
	Vec3 Pos;
	memcpy(&Pos, Vertices, sizeof(Vec3));

	for (unsigned int i = 0; i < Count; i++)
	{
		memcpy(&Pos, Vertices + _Size * i, sizeof(Vec3));
		if (Min.x > Pos.x)
		{
			Min.x = Pos.x;
		}

		if (Min.y > Pos.y)
		{
			Min.y = Pos.y;
		}

		if (Min.z > Pos.z)
		{
			Min.z = Pos.z;
		}

		if (Max.x < Pos.x)
		{
			Max.x = Pos.x;
		}

		if (Max.y < Pos.y)
		{
			Max.y = Pos.y;
		}

		if (Max.z < Pos.z)
		{
			Max.z = Pos.z;
		}
	}

	Center = (Min + Max) / 2.0f;
	Length = Max - Min;
	Radius = Length.Length() / 2.0f;

	return true;
}

bool AIMMesh::CreateIndexBuffer(int _Size, int _Count, D3D11_USAGE _Usage, DXGI_FORMAT _Format, void * _Data)
{
	MeshContainer* Container = MeshContainerVec.back();

	IndexBuffer* IB = new IndexBuffer;

	Container->IBVec.push_back(IB);

	D3D11_BUFFER_DESC Desc = {};

	Desc.ByteWidth = _Size * _Count;
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Desc.Usage = _Usage;

	switch (_Usage)
	{
	case D3D11_USAGE_DYNAMIC:
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	IB->Size = _Size;
	IB->Count = _Count;
	IB->Usage = _Usage;
	IB->Format = _Format;
	IB->Data = new char[_Size * _Count];

	memcpy(IB->Data, _Data, _Size * _Count);

	D3D11_SUBRESOURCE_DATA Data = {};

	Data.pSysMem = IB->Data;

	if (FAILED(GetAIMDevice->CreateBuffer(&Desc, &Data, &IB->Buffer)))
	{
		tassertmsg(true, "IndexBuffer Create Failed");
		return false;
	}

	return true;
}

bool AIMMesh::ConvertFbx(FbxLoader * _Loader, const char * _FullPath)
{
	const std::vector<Ezptr<FbxMeshContainer>>* ContainerVec = _Loader->GetMeshContainers();
	const std::vector<std::vector<Ezptr<FbxMaterial>>>* MaterialVec = _Loader->GetMaterials();

	std::vector<Ezptr<FbxMeshContainer>>::const_iterator MeshContainerStartIter = ContainerVec->begin();
	std::vector<Ezptr<FbxMeshContainer>>::const_iterator MeshContainerEndIter = ContainerVec->end();

	std::vector<bool> EmptyIndexVec;
	bool bAnimation = false;

	for (; MeshContainerStartIter != MeshContainerEndIter; ++MeshContainerStartIter)
	{
		MeshContainer* Container = new MeshContainer;

		InputLayoutName = "StandardVertex3DLayout";

		MeshContainerVec.push_back(Container);

		int VtxSize = 0;

		if ((*MeshContainerStartIter)->Bump == true)
		{
			if ((*MeshContainerStartIter)->Animation == true)
			{
				bAnimation = true;
				// 범프 애니메이션 셰이더
			}
			else
			{
				ShaderName = "Standard3DShader";
			}
		}
		else
		{
			if ((*MeshContainerStartIter)->Animation == true)
			{
				bAnimation = true;
				// 애니메이션 셰이더
			}
			else
			{
				ShaderName = "";
			}
		}

		ShaderName = "Standard3DShader";

		std::vector<Vertex3D> VtxVec;
		VtxSize = sizeof(Vertex3D);

		for (size_t i = 0; i < (*MeshContainerStartIter)->PosVec.size(); i++)
		{
			Vertex3D Vtx = {};

			Vtx.Pos = (*MeshContainerStartIter)->PosVec[i];
			Vtx.Normal = (*MeshContainerStartIter)->NormalVec[i];
			Vtx.UV = (*MeshContainerStartIter)->UVVec[i];

			if ((*MeshContainerStartIter)->TangentVec.empty() == false)
			{
				Vtx.Tangent = (*MeshContainerStartIter)->TangentVec[i];
			}

			if ((*MeshContainerStartIter)->BinormalVec.empty() == false)
			{
				Vtx.Binormal = (*MeshContainerStartIter)->BinormalVec[i];
			}

			if ((*MeshContainerStartIter)->BlendWeightVec.empty() == false)
			{
				Vtx.BlendWeights = (*MeshContainerStartIter)->BlendWeightVec[i];
				Vtx.BlendIndices = (*MeshContainerStartIter)->BlendIndexVec[i];
			}

			VtxVec.push_back(Vtx);
		}

		Container->VB.Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		if (false == CreateVertexBuffer(VtxSize, (int)VtxVec.size(), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &VtxVec[0]))
		{
			return false;
		}

		for (size_t i = 0; i < (*MeshContainerStartIter)->IndicesVec.size(); i++)
		{
			if ((*MeshContainerStartIter)->IndicesVec[i].empty() == true)
			{
				EmptyIndexVec.push_back(false);
				continue;
			}

			EmptyIndexVec.push_back(true);

			if (false == CreateIndexBuffer(4, (int)(*MeshContainerStartIter)->IndicesVec[i].size(), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, (void*)&(*MeshContainerStartIter)->IndicesVec[i][0]))
			{
				return false;
			}
		}
	}

	const std::vector<std::vector<Ezptr<FbxMaterial>>>* Materials = _Loader->GetMaterials();

	std::vector<std::vector<Ezptr<FbxMaterial>>>::const_iterator MaterialStartIter = Materials->begin();
	std::vector<std::vector<Ezptr<FbxMaterial>>>::const_iterator MaterialEndIter = Materials->end();

	if (Materials->empty() == false)
	{
		Material = new AIMMaterial;

		if (Material->Init() == false)
		{
			Material = nullptr;
			return NULL;
		}
	}

	int Container = 0;
	for (; MaterialStartIter != MaterialEndIter  ; ++MaterialStartIter)
	{
		for (size_t i = 0; i < (*MaterialStartIter).size(); i++)
		{
			if (EmptyIndexVec[i] == false)
			{
				continue;
			}

			Ezptr<FbxMaterial> Mtrl = (*MaterialStartIter)[i];

			Material->SetColor(Mtrl->Dif, Mtrl->Amb, Mtrl->Spc, Mtrl->Emv, Mtrl->SpcPow, Container, i);

			if (bAnimation == true)
			{
				Material->Skinning(Container, i);
			}

			char Name[MAX_PATH] = {};

			_splitpath_s(Mtrl->DifTex.c_str(), NULL, 0, NULL, 0, Name, MAX_PATH, NULL, 0);

			TCHAR Path[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, Mtrl->DifTex.c_str(), -1, Path, (int)Mtrl->DifTex.length());
#else
			strcpy_s(Path, Mtrl->DifTex.c_str());
#endif

			Material->AddTextureSetFromFullPath(Container, (int)i, 0, Name, Path);

			if (false == Mtrl->BumpTex.empty())
			{
				memset(Name, 0, MAX_PATH);
				_splitpath_s(Mtrl->BumpTex.c_str(), NULL, 0, NULL, 0, Name, MAX_PATH, NULL, 0);

				memset(Path, 0, sizeof(wchar_t) * MAX_PATH);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, Mtrl->BumpTex.c_str(), -1, Path, (int)Mtrl->BumpTex.length());
#else
				strcpy_s(Path, Mtrl->BumpTex.c_str());
#endif

				Material->AddTextureSetFromFullPath(Container, (int)i, 1, Name, Path);

				Material->BumpTextureEnable(Container, i);
			}

			if (false == Mtrl->SpcTex.empty())
			{
				memset(Name, 0, MAX_PATH);
				_splitpath_s(Mtrl->SpcTex.c_str(), NULL, 0, NULL, 0, Name, MAX_PATH, NULL, 0);

				memset(Path, 0, sizeof(wchar_t) * MAX_PATH);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, Mtrl->SpcTex.c_str(), -1, Path, (int)Mtrl->SpcTex.length());
#else
				strcpy_s(Path, Mtrl->SpcTex.c_str());
#endif
				Material->AddTextureSetFromFullPath(Container, (int)i, 2, Name, Path);

				Material->SpecularTextureEnable(Container, i);
			}
		}
	}

	Length = Max - Min;

	Center = (Max + Min) / 2.0f;
	Radius = (Length.Length() / 2.0f);

	char FullPath[MAX_PATH] = {};
	strcpy_s(FullPath, _FullPath);
	int PathLength = strlen(FullPath);
	memcpy(&FullPath[PathLength - 4], ".amsh", 6);

	SaveFullPath(FullPath);

	const std::vector<Ezptr<FbxBone>>* BoneVec = _Loader->GetBoneVec();

	if (BoneVec->empty() == true)
	{
		return true;
	}

	Animation = new AIMAnimation;

	if (false == Animation->Init())
	{
		Animation = nullptr;
		return false;
	}

	std::vector<Ezptr<FbxBone>>::const_iterator BoneStartIter = BoneVec->begin();
	std::vector<Ezptr<FbxBone>>::const_iterator BoneEndIter = BoneVec->end();

	for ( ; BoneStartIter != BoneEndIter ; ++BoneStartIter)
	{
		Ezptr<Bone> TmpBone = new Bone;

		TmpBone->Name = (*BoneStartIter)->Name;
		TmpBone->Depth = (*BoneStartIter)->Depth;
		TmpBone->ParendIndex = (*BoneStartIter)->ParentIndex;

		float Mat[4][4] = {};

		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				Mat[y][x] = (*BoneStartIter)->OffsetMat.mData[y].mData[x];
			}
		}

		TmpBone->OffsetMat = new Matrix;
		*(TmpBone->OffsetMat) = Mat;

		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				Mat[y][x] = (*BoneStartIter)->BoneMat.mData[y].mData[x];
			}
		}

		TmpBone->BoneMat = new Matrix;
		*(TmpBone->BoneMat) = Mat;

		Animation->AddBone(TmpBone);
	}

	Animation->CreateBoneTexture();

	const std::vector<Ezptr<FbxAnimationClip>>* ClipVec = _Loader->GetClipVec();

	std::vector<Ezptr<FbxAnimationClip>>::const_iterator StartIter = ClipVec->begin();
	std::vector<Ezptr<FbxAnimationClip>>::const_iterator EndIter = ClipVec->end();

	for (  ;  StartIter != EndIter ; ++StartIter)
	{
		Animation->AddClip(AO_LOOP, (*StartIter));
	}

	if (Animation != nullptr)
	{
		memcpy(&FullPath[PathLength - 3], "bne", 3);
		Animation->SaveBoneFromFullPath(FullPath);

		memcpy(&FullPath[PathLength - 3], "anm", 3);
		Animation->SaveFromFullPath(FullPath);
	}



	return true;
}

bool AIMMesh::Save(const char * _FileName, const std::string & _Path)
{
	char FullPath[MAX_PATH] = {};
	const char* Path = PathManager::FindPathMultiByte(_Path);

	if (Path != nullptr)
	{
		strcpy_s(FullPath, Path);
	}

	strcat_s(FullPath, _FileName);

	return SaveFullPath(FullPath);
}

bool AIMMesh::SaveFullPath(const char * _FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, _FullPath, "wb");

	if (File == nullptr)
	{
		tassertmsg(true, "Failed To Save Mesh");
		return false;
	}

	int TmpLength = NameTag.length();

	// 이름 저장
	fwrite(&TmpLength, 4, 1, File);
	fwrite(NameTag.c_str(), 1, TmpLength, File);

	// 셰이더 이름 저장
	TmpLength = ShaderName.length();
	fwrite(&TmpLength, 4, 1, File);
	fwrite(ShaderName.c_str(), 1, TmpLength, File);

	// 입력 레이아웃 이름 저장
	TmpLength = InputLayoutName.length();
	fwrite(&TmpLength, 4, 1, File);
	fwrite(InputLayoutName.c_str(), 1, TmpLength, File);

	fwrite(&Center, sizeof(Vec3), 1, File);
	fwrite(&Radius, sizeof(float), 1, File);
	fwrite(&Min, sizeof(Vec3), 1, File);
	fwrite(&Max, sizeof(Vec3), 1, File);
	fwrite(&Length, sizeof(Vec3), 1, File);
	
	size_t ContainerSize = MeshContainerVec.size();

	fwrite(&ContainerSize, sizeof(size_t), 1, File);

	for (size_t AA = 0; AA < ContainerSize; AA++)
	{
		MeshContainer* Container = MeshContainerVec[AA];

		fwrite(&Container->VB.Primitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, File);

		fwrite(&Container->VB.Size, sizeof(int), 1, File);
		fwrite(&Container->VB.Count, sizeof(int), 1, File);
		fwrite(&Container->VB.Usage, sizeof(D3D11_USAGE), 1, File);
		fwrite(Container->VB.Data, Container->VB.Size, Container->VB.Count, File);

		size_t IdxCount = Container->IBVec.size();

		fwrite(&IdxCount, sizeof(size_t), 1, File);

		for (size_t BB = 0; BB < IdxCount; BB++)
		{
			fwrite(&Container->IBVec[BB]->Format, sizeof(DXGI_FORMAT), 1, File);
			fwrite(&Container->IBVec[BB]->Size, sizeof(int), 1, File);
			fwrite(&Container->IBVec[BB]->Count, sizeof(int), 1, File);
			fwrite(&Container->IBVec[BB]->Usage, sizeof(D3D11_USAGE), 1, File);
			fwrite(Container->IBVec[BB]->Data, Container->IBVec[BB]->Size, Container->IBVec[BB]->Count, File);
		}
	}

	bool bMaterial = false;

	if (Material != nullptr)
	{
		bMaterial = true;

		fwrite(&bMaterial, sizeof(bool), 1, File);

		Material->Save(File);
	}
	else
	{
		fwrite(&bMaterial, sizeof(bool), 1, File);
	}

	fclose(File);

	return true;
}

bool AIMMesh::Load(const char * _FileName, const std::string & _Path)
{
	char FullPath[MAX_PATH] = {};

	const char* Path = PathManager::FindPathMultiByte(_Path);

	if (Path != nullptr)
	{
		strcpy_s(FullPath, Path);
	}

	strcat_s(FullPath, _FileName);

	return LoadFullPath(FullPath);
}

bool AIMMesh::LoadFullPath(const char * _FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, _FullPath, "rb");

	if (File == nullptr)
	{
		tassertmsg(true, "Failed To Load Mesh");
		return false;
	}

	int TmpLength = 0;

	fread(&TmpLength, 4, 1, File);

	char Name[256] = {};
	fread(Name, 1, TmpLength, File);
	NameTag = Name;

	TmpLength = 0;
	fread(&TmpLength, 4, 1, File);
	char Shader[256] = {};
	fread(Shader, 1, TmpLength, File);
	ShaderName = Shader;

	TmpLength = 0;
	fread(&TmpLength, 4, 1, File);
	char Layout[256] = {};
	fread(Layout, 1, TmpLength, File);
	InputLayoutName = Layout;

	fread(&Center, sizeof(Vec3), 1, File);
	fread(&Radius, sizeof(float), 1, File);
	fread(&Min, sizeof(Vec3), 1, File);
	fread(&Max, sizeof(Vec3), 1, File);
	fread(&Length, sizeof(Vec3), 1, File);

	size_t ContainerCount = 0;

	fread(&ContainerCount, sizeof(size_t), 1, File);

	for (size_t AA = 0; AA < ContainerCount; AA++)
	{
		MeshContainer* Container = new MeshContainer;
		MeshContainerVec.push_back(Container);

		int VtxSize = 0;
		int VtxCount = 0;
		D3D11_USAGE Usage;

		fread(&Container->VB.Primitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, File);
		fread(&VtxSize, sizeof(int), 1, File);
		fread(&VtxCount, sizeof(int), 1, File);
		fread(&Usage, sizeof(D3D11_USAGE), 1, File);
		char* TmpData = new char[VtxSize * VtxCount];
		fread(TmpData, VtxSize, VtxCount, File);

		CreateVertexBuffer(VtxSize, VtxCount, Usage, Container->VB.Primitive, TmpData);

		if (TmpData != nullptr)
		{
			delete[] TmpData;
			TmpData = nullptr;
		}

		size_t IdxCount = 0;

		fread(&IdxCount, sizeof(size_t), 1, File);

		for (size_t BB = 0; BB < IdxCount; BB++)
		{
			DXGI_FORMAT Format;
			int IdxSize = 0;
			int IdxCount = 0;
			fread(&Format, sizeof(DXGI_FORMAT), 1, File);
			fread(&IdxSize, sizeof(int), 1, File);
			fread(&IdxCount, sizeof(int), 1, File);
			fread(&Usage, sizeof(D3D11_USAGE), 1, File);

			TmpData = new char[IdxCount * IdxSize];

			fread(TmpData, IdxSize, IdxCount, File);

			CreateIndexBuffer(IdxSize, IdxCount, Usage, Format, TmpData);

			if (TmpData != nullptr)
			{
				delete[] TmpData;
				TmpData = nullptr;
			}
		}
	}

	bool bMaterial = false;
	fread(&bMaterial, sizeof(bool), 1, File);

	if (bMaterial == true)
	{
		bMaterial = true;

		Material = new AIMMaterial;
		Material->Init();

		Material->Load(File);
	}

	fclose(File);

	return true;
}

Ezptr<AIMMaterial> AIMMesh::CloneMaterial()
{
	if (Material == nullptr)
	{
		return nullptr;
	}

	return Material->Clone();
}

Ezptr<AIMAnimation> AIMMesh::CloneAnimation()
{
	if (Animation == nullptr)
	{
		return nullptr;
	}

	return Animation->Clone();
}
