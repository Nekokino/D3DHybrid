#include "FbxLoader.h"
#include <algorithm>


FbxLoader::FbxLoader()
{
}


FbxLoader::~FbxLoader()
{
	MaterialVec.clear();
	MeshContainerVec.clear();
	NameArray.Clear();
	Scene->Clear();
	Scene->Destroy();
	Manager->Destroy();
}

const std::vector<Ezptr<FbxMeshContainer>>* FbxLoader::GetMeshContainers() const
{
	return &MeshContainerVec;
}

const std::vector<std::vector<Ezptr<FbxMaterial>>>* FbxLoader::GetMaterials() const
{
	return &MaterialVec;
}

const std::vector<Ezptr<FbxBone>>* FbxLoader::GetBoneVec() const
{
	return &BoneVec;
}

const std::vector<Ezptr<FbxAnimationClip>>* FbxLoader::GetClipVec() const
{
	return &ClipVec;
}

bool FbxLoader::LoadFbx(const char * _FullPath, FbxLoadType _Type)
{
	LoadType = _Type;
	// FbxManager객체 생성
	Manager = FbxManager::Create();
	
	// IOSetting 생성
	FbxIOSettings* Ios = FbxIOSettings::Create(Manager, IOSROOT);

	// FbxManager에 지정
	Manager->SetIOSettings(Ios);

	// FbxScene 생성
	Scene = FbxScene::Create(Manager, "");

	// FbxImporter 이용해 Fbx파일로부터 메쉬 정보 읽기
	FbxImporter* Importer = FbxImporter::Create(Manager, "");

	// 경로에 있는 Fbx 파일 읽고 정보를 만든다
	Importer->Initialize(_FullPath, -1, Manager->GetIOSettings());

	// 만들어낸 정보를 FbxScene에 노드를 구성해준다
	Importer->Import(Scene);

	if (Scene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
	{
		// 축정보가 Max기반 정보가 아니라면 Max기반으로 설정해준다
		Scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);
	}

	Scene->FillAnimStackNameArray(NameArray);

	if (NameArray.GetCount() > 0)
	{
		LoadAnimationClip();

		LoadBone(Scene->GetRootNode());

		for (size_t i = 0; i < ClipVec.size(); i++)
		{
			ClipVec[i]->BoneKeyFrameVec.resize(BoneVec.size());
		}
	}

	Triangulate(Scene->GetRootNode());

	LoadMesh(Scene->GetRootNode());

	Importer->Destroy();

	return true;

}

void FbxLoader::Triangulate(FbxNode * _Node)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();

	if (Attr != nullptr && (Attr->GetAttributeType() == FbxNodeAttribute::eMesh || Attr->GetAttributeType() == FbxNodeAttribute::eNurbs || Attr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter Converter(Manager);

		Converter.Triangulate(Attr, true);
	}

	// 재질정보 읽기
	int MtrlCount = _Node->GetMaterialCount();

	if (MtrlCount > 0)
	{
		std::vector<Ezptr<FbxMaterial>> MtrlVec;

		MaterialVec.push_back(MtrlVec);

		for (int i = 0; i < MtrlCount; i++)
		{
			FbxSurfaceMaterial* Mtrl = _Node->GetMaterial(i);

			if (Mtrl == nullptr)
			{
				continue;
			}

			LoadMaterial(Mtrl);
		}
	}

	// 현재 노드의 자식노드 수 구한다
	int ChildCount = _Node->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		Triangulate(_Node->GetChild(i));
	}
}

void FbxLoader::LoadMaterial(FbxSurfaceMaterial * _Mtrl)
{
	Ezptr<FbxMaterial> MtrlInfo = new FbxMaterial;

	// 마지막 컨테이너에 추가
	MaterialVec[MaterialVec.size() - 1].push_back(MtrlInfo);

	// Diffuse 정보 읽기
	MtrlInfo->Dif = GetMaterialColor(_Mtrl, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	MtrlInfo->Amb = GetMaterialColor(_Mtrl, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	MtrlInfo->Spc = GetMaterialColor(_Mtrl, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	MtrlInfo->Emv = GetMaterialColor(_Mtrl, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

	MtrlInfo->SpcPow = (float)GetMaterialFactor(_Mtrl, FbxSurfaceMaterial::sSpecularFactor);
	MtrlInfo->Shininess = (float)GetMaterialFactor(_Mtrl, FbxSurfaceMaterial::sShininess);
	MtrlInfo->TransparencyFactor = (float)GetMaterialFactor(_Mtrl, FbxSurfaceMaterial::sTransparencyFactor);

	MtrlInfo->DifTex = GetMaterialTexture(_Mtrl, FbxSurfaceMaterial::sDiffuse);
	MtrlInfo->BumpTex = GetMaterialTexture(_Mtrl, FbxSurfaceMaterial::sNormalMap);

	if (MtrlInfo->BumpTex.empty() == true)
	{
		MtrlInfo->BumpTex = GetMaterialTexture(_Mtrl, FbxSurfaceMaterial::sBump);
	}

	MtrlInfo->SpcTex = GetMaterialTexture(_Mtrl, FbxSurfaceMaterial::sSpecular);
}

Vec4 FbxLoader::GetMaterialColor(FbxSurfaceMaterial * _Mtrl, const char * _PropertyName, const char * _PropertyFactorName)
{
	FbxDouble3 Result(0, 0, 0);
	double Factor = 0;

	// 인자로 들어온 재질에서 해당 프로퍼티와 프로퍼티 팩터가 있는지 확인 후 가져온다.

	FbxProperty Property = _Mtrl->FindProperty(_PropertyName);
	FbxProperty PropertyFactor = _Mtrl->FindProperty(_PropertyFactorName);

	if (Property.IsValid() == true && PropertyFactor.IsValid() == true)
	{
		Result = Property.Get<FbxDouble3>();
		Factor = PropertyFactor.Get<FbxDouble>();

		// Factor가 1이 아닐경우 비율값으로 사용해 Result에 곱해준다
		if (Factor != 1)
		{ 
			Result[0] *= Factor;
			Result[1] *= Factor;
			Result[2] *= Factor;
		}
	}

	return Vec4((float)Result[0], (float)Result[1], (float)Result[2], (float)Factor);
}

double FbxLoader::GetMaterialFactor(FbxSurfaceMaterial * _Mtrl, const char * _PropertyName)
{
	FbxProperty Property = _Mtrl->FindProperty(_PropertyName);
	double Factor = 0;

	if (Property.IsValid() == true)
	{
		Factor = Property.Get<FbxDouble>();
	}

	return Factor;
}

std::string FbxLoader::GetMaterialTexture(FbxSurfaceMaterial * _Mtrl, const char * _PropertyName)
{
	FbxProperty Property = _Mtrl->FindProperty(_PropertyName);

	std::string Return = "";

	if (Property.IsValid() == true)
	{
		// FbxFileTexture에 타입이 몇개인지 얻어온다.
		int TexCount = Property.GetSrcObjectCount<FbxFileTexture>();

		if (TexCount > 0)
		{
			FbxFileTexture* FileTex = Property.GetSrcObject<FbxFileTexture>(0);

			if (FileTex != nullptr)
			{
				Return = FileTex->GetFileName();
			}
		}
	}

	return Return;
}

void FbxLoader::LoadMesh(FbxNode * _Node)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();

	if (Attr != nullptr && Attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* Mesh = _Node->GetMesh();

		if (Mesh != nullptr)
		{
			LoadMesh(Mesh);
		}
	}

	int ChildCount = _Node->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		LoadMesh(_Node->GetChild(i));
	}
}

void FbxLoader::LoadMesh(FbxMesh * _Mesh)
{
	Ezptr<FbxMeshContainer> Container = new FbxMeshContainer;

	Container->Bump = false;

	MeshContainerVec.push_back(Container);

	// 위치정보를 담는 배열의 갯수 = 정점의 갯수
	int VtxCount = _Mesh->GetControlPointsCount();

	// 배열의 시작주소를 가져온다
	FbxVector4* VtxPos = _Mesh->GetControlPoints();

	Container->PosVec.resize(VtxCount);
	Container->NormalVec.resize(VtxCount);
	Container->UVVec.resize(VtxCount);
	Container->TangentVec.resize(VtxCount);
	Container->BinormalVec.resize(VtxCount);

	for (int i = 0; i < VtxCount; i++)
	{
		// y와 z축 교환
		Container->PosVec[i].x = (float)VtxPos[i].mData[0];
		Container->PosVec[i].y = (float)VtxPos[i].mData[2];
		Container->PosVec[i].z = (float)VtxPos[i].mData[1];
	}

	// 폴리곤 수
	int PolygonCount = _Mesh->GetPolygonCount();

	
	UINT VtxID = 0;

	// 재질 수
	int MtrlCount = _Mesh->GetNode()->GetMaterialCount();

	// 재질 수는 곧 서브셋 수
	Container->IndicesVec.resize(MtrlCount);

	// 재질 정보를 가져온다
	FbxGeometryElementMaterial* Material = _Mesh->GetElementMaterial();

	int Count = _Mesh->GetElementMaterialCount();

	//삼각형 수만큼 반복
	for (int AA = 0; AA < PolygonCount; AA++)
	{
		// 폴리곤을 구성하는 정점의 개수
		// 삼각형이니 세개
		int PolygonSize = _Mesh->GetPolygonSize(AA);

		int Idx[3] = {};

		for (int BB = 0; BB < PolygonSize; BB++)
		{
			int ControlIndex = _Mesh->GetPolygonVertex(AA, BB);

			Idx[BB] = ControlIndex;

			LoadNormal(_Mesh, Container, VtxID, ControlIndex);
			LoadUV(_Mesh, Container, _Mesh->GetTextureUVIndex(AA, BB), ControlIndex);
			LoadTangent(_Mesh, Container, VtxID, ControlIndex);
			LoadBinormal(_Mesh, Container, VtxID, ControlIndex);

			++VtxID;
		}

		int MtrlID = Material->GetIndexArray().GetAt(AA);

		Container->IndicesVec[MtrlID].push_back(Idx[0]);
		Container->IndicesVec[MtrlID].push_back(Idx[2]);
		Container->IndicesVec[MtrlID].push_back(Idx[1]);
	}

	LoadAnimation(_Mesh, Container);
}

void FbxLoader::LoadNormal(FbxMesh * _Mesh, Ezptr<FbxMeshContainer> _Container, int _VtxID, int _ControlIndex)
{
	FbxGeometryElementNormal* Normal = _Mesh->GetElementNormal();

	if (Normal == nullptr)
	{
		return;
	}

	int NormalIndex = _VtxID;

	if (Normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (Normal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			NormalIndex = Normal->GetIndexArray().GetAt(_VtxID);
			break;
		}
	}
	else if (Normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (Normal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			NormalIndex = _ControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			NormalIndex = Normal->GetIndexArray().GetAt(_ControlIndex);
			break;
		}
	}

	FbxVector4 NormalVector = Normal->GetDirectArray().GetAt(NormalIndex);

	_Container->NormalVec[_ControlIndex].x = (float)NormalVector.mData[0];
	_Container->NormalVec[_ControlIndex].y = (float)NormalVector.mData[2];
	_Container->NormalVec[_ControlIndex].z = (float)NormalVector.mData[1];
}

void FbxLoader::LoadUV(FbxMesh * _Mesh, Ezptr<FbxMeshContainer> _Container, int _UVID, int _ControlIndex)
{
	int Count = _Mesh->GetElementUVCount();
	FbxGeometryElementUV* UV = _Mesh->GetElementUV(0);

	if (UV == nullptr)
	{
		return;
	}

	int UVIndex = _UVID;

	if (UV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (UV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			UVIndex = _ControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			UVIndex = UV->GetIndexArray().GetAt(_ControlIndex);
			break;
		}
	}
	else if (UV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (UV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
			// UVIndex = UV->GetIndexArray().GetAt(UVIndex);
			break;
		default:
			break;
		}
	}

	FbxVector2 UVVector = UV->GetDirectArray().GetAt(UVIndex);

	_Container->UVVec[_ControlIndex].x = (float)(UVVector.mData[0] - (int)UVVector.mData[0]);
	_Container->UVVec[_ControlIndex].y = (float)(1.0f - (UVVector.mData[1] - (int)UVVector.mData[1]));
}

void FbxLoader::LoadTangent(FbxMesh * _Mesh, Ezptr<FbxMeshContainer> _Container, int _VtxID, int _ControlIndex)
{
	FbxGeometryElementTangent* Tangent = _Mesh->GetElementTangent();

	if (Tangent == nullptr)
	{
		return;
	}

	_Container->Bump = true;

	int TangentIndex = _VtxID;

	if (Tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (Tangent->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			TangentIndex = Tangent->GetIndexArray().GetAt(_VtxID);
			break;
		}
	}
	else if (Tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (Tangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			TangentIndex = _ControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			TangentIndex = Tangent->GetIndexArray().GetAt(_ControlIndex);
			break;
		}
	}

	FbxVector4 TangentVector = Tangent->GetDirectArray().GetAt(TangentIndex);

	_Container->TangentVec[_ControlIndex].x = (float)TangentVector.mData[0];
	_Container->TangentVec[_ControlIndex].y = (float)TangentVector.mData[2];
	_Container->TangentVec[_ControlIndex].z = (float)TangentVector.mData[1];
}

void FbxLoader::LoadBinormal(FbxMesh * _Mesh, Ezptr<FbxMeshContainer> _Container, int _VtxID, int _ControlIndex)
{
	FbxGeometryElementBinormal* Binormal = _Mesh->GetElementBinormal();

	if (Binormal == nullptr)
	{
		return;
	}

	_Container->Bump = true;

	int BinormalIndex = _VtxID;

	if (Binormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (Binormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			BinormalIndex = Binormal->GetIndexArray().GetAt(_VtxID);
			break;
		}
	}

	else if (Binormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (Binormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			BinormalIndex = _ControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			BinormalIndex = Binormal->GetIndexArray().GetAt(_ControlIndex);
			break;
		}
	}

	FbxVector4 BinormalVector = Binormal->GetDirectArray().GetAt(BinormalIndex);

	_Container->BinormalVec[_ControlIndex].x = (float)BinormalVector.mData[0];
	_Container->BinormalVec[_ControlIndex].y = (float)BinormalVector.mData[2];
	_Container->BinormalVec[_ControlIndex].z = (float)BinormalVector.mData[1];
}

void FbxLoader::LoadAnimationClip()
{
	int Count = NameArray.GetCount();

	FbxTime::EMode TimeMode = Scene->GetGlobalSettings().GetTimeMode();

	for (int i = 0; i < Count; i++)
	{
		FbxAnimStack* AnimStack = Scene->FindMember<FbxAnimStack>(NameArray[i]->Buffer());

		if (AnimStack == nullptr)
		{
			continue;
		}

		Ezptr<FbxAnimationClip> Clip = new FbxAnimationClip;

		Clip->Name = AnimStack->GetName();

		if (Clip->Name == "mixamo.com")
		{
			Mixamo = true;
		}

		FbxTakeInfo* Take = Scene->GetTakeInfo(Clip->Name.c_str());

		Clip->Start = Take->mLocalTimeSpan.GetStart();
		Clip->End = Take->mLocalTimeSpan.GetStop();

		Clip->TimeLength = Clip->End.GetFrameCount(TimeMode) - Clip->Start.GetFrameCount(TimeMode);

		Clip->TimeMode = TimeMode;

		ClipVec.push_back(Clip);
	}
}

void FbxLoader::LoadBone(FbxNode * _Node)
{
	int ChildCount = _Node->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		LoadBoneRecursive(_Node->GetChild(i), 0, 0, -1);
	}
}

void FbxLoader::LoadBoneRecursive(FbxNode * _Node, int _Depth, int _Index, int _Parent)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();

	if (Attr != nullptr && Attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Ezptr<FbxBone> Bone = new FbxBone;

		Bone->Name = _Node->GetName();

		if (Mixamo == true)
		{
			Bone->Name.erase(0, 10);
		}

		Bone->Depth = _Depth;
		Bone->ParentIndex = _Parent;

		BoneVec.push_back(Bone);
	}

	int ChildCount = _Node->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		LoadBoneRecursive(_Node->GetChild(i), _Depth + 1, (int)BoneVec.size(), _Index);
	}
}

void FbxLoader::LoadAnimation(FbxMesh * _Mesh, Ezptr<FbxMeshContainer> _Container)
{
	int SkinCount = _Mesh->GetDeformerCount(FbxDeformer::eSkin);

	if (SkinCount <= 0)
	{
		return;
	}

	int CPCount = _Mesh->GetControlPointsCount();

	// 정점의 가중치와 인덱스 정보는 정점갯수만큼 생성
	_Container->BlendWeightVec.resize(CPCount);
	_Container->BlendIndexVec.resize(CPCount);

	_Container->Animation = true;
	FbxAMatrix TransformMat = GetTransform(_Mesh->GetNode());

	for (int AA = 0; AA < SkinCount; AA++)
	{
		FbxSkin* Skin = (FbxSkin*)_Mesh->GetDeformer(AA, FbxDeformer::eSkin);

		if (Skin == nullptr)
		{
			continue;
		}

		FbxSkin::EType SkinningType = Skin->GetSkinningType();

		if (SkinningType == FbxSkin::eRigid || SkinningType == FbxSkin::eLinear)
		{
			int ClusterCount = Skin->GetClusterCount();

			for (int BB = 0; BB < ClusterCount; BB++)
			{
				FbxCluster* Cluster = Skin->GetCluster(BB);

				if (Cluster->GetLink() == nullptr)
				{
					continue;
				}

				std::string BoneName = Cluster->GetLink()->GetName();

				if (Mixamo == true)
				{
					BoneName.erase(0, 10);
				}

				int BoneIndex = FindBoneFromName(BoneName);

				LoadWeightAndIndex(Cluster, BoneIndex, _Container);

				LoadOffsetMatrix(Cluster, TransformMat, BoneIndex, _Container);

				BoneVec[BoneIndex]->BoneMat = TransformMat;

				LoadTimeTransform(_Mesh->GetNode(), Cluster, TransformMat, BoneIndex);
			}
		}
	}

	ChangeWeightAndIndices(_Container);
}

FbxAMatrix FbxLoader::GetTransform(FbxNode * _Node)
{
	const FbxVector4 T = _Node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 R = _Node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 S = _Node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(T, R, S);
}

int FbxLoader::FindBoneFromName(const std::string & _Name)
{
	for (size_t i = 0; i < BoneVec.size(); i++)
	{
		if (BoneVec[i]->Name == _Name)
		{
			return (int)i;
		}
	}

	return -1;
}

void FbxLoader::LoadWeightAndIndex(FbxCluster * _Cluster, int _BoneIndex, Ezptr<FbxMeshContainer> _Container)
{
	int ControlIndicesCount = _Cluster->GetControlPointIndicesCount();

	for (int i = 0; i < ControlIndicesCount; i++)
	{
		FbxWeight Weight = {};

		Weight.Index = _BoneIndex;
		Weight.Weight = _Cluster->GetControlPointWeights()[i];

		int ClusterIndex = _Cluster->GetControlPointIndices()[i];

		_Container->WeightMap[ClusterIndex].push_back(Weight);
	}
}

void FbxLoader::LoadOffsetMatrix(FbxCluster * _Cluster, const FbxAMatrix & _TransformMat, int _BoneIndex, Ezptr<FbxMeshContainer> _Container)
{
	FbxAMatrix ClusterMat;
	FbxAMatrix ClusterLinkMat;

	_Cluster->GetTransformMatrix(ClusterMat);
	_Cluster->GetTransformLinkMatrix(ClusterLinkMat);

	FbxVector4 _1 = { 1.0f, 0.0f, 0.0f, 0.0f };
	FbxVector4 _2 = { 0.0f, 0.0f, 1.0f, 0.0f };
	FbxVector4 _3 = { 0.0f, 1.0f, 0.0f, 0.0f };
	FbxVector4 _4 = { 0.0f, 0.0f, 0.0f, 1.0f };

	FbxAMatrix ReflectMat;
	ReflectMat.mData[0] = _1;
	ReflectMat.mData[1] = _2;
	ReflectMat.mData[2] = _3;
	ReflectMat.mData[3] = _4;

	FbxAMatrix OffsetMat;

	OffsetMat = ClusterLinkMat.Inverse() * ClusterMat * _TransformMat;
	OffsetMat = ReflectMat * OffsetMat * ReflectMat;

	BoneVec[_BoneIndex]->OffsetMat = OffsetMat;
}

void FbxLoader::LoadTimeTransform(FbxNode * _Node, FbxCluster * _Cluster, const FbxAMatrix & _TransformMat, int _BoneIndex)
{
	FbxVector4 _1 = { 1.0f, 0.0f, 0.0f, 0.0f };
	FbxVector4 _2 = { 0.0f, 0.0f, 1.0f, 0.0f };
	FbxVector4 _3 = { 0.0f, 1.0f, 0.0f, 0.0f };
	FbxVector4 _4 = { 0.0f, 0.0f, 0.0f, 1.0f };

	FbxAMatrix ReflectMat;
	ReflectMat.mData[0] = _1;
	ReflectMat.mData[1] = _2;
	ReflectMat.mData[2] = _3;
	ReflectMat.mData[3] = _4;

	if (Mixamo == true)
	{
		std::vector<Ezptr<FbxAnimationClip>>::iterator StartIter = ClipVec.begin();
		std::vector<Ezptr<FbxAnimationClip>>::iterator EndIter = ClipVec.end();

		for (; StartIter != EndIter;)
		{
			if ((*StartIter)->Name != "mixamo.com")
			{
				StartIter = ClipVec.erase(StartIter);
				EndIter = ClipVec.end();
			}
			else
				++StartIter;
		}
	}

	for (size_t AA = 0; AA < ClipVec.size(); AA++)
	{
		FbxLongLong Start = ClipVec[AA]->Start.GetFrameCount(ClipVec[AA]->TimeMode);
		FbxLongLong End = ClipVec[AA]->End.GetFrameCount(ClipVec[AA]->TimeMode);

		ClipVec[AA]->BoneKeyFrameVec[_BoneIndex].BoneIndex = _BoneIndex;

		for (FbxLongLong BB = Start; BB <= End; BB++)
		{
			FbxTime Time = {};

			Time.SetFrame(BB, ClipVec[AA]->TimeMode);

			FbxAMatrix OffsetMat = _Node->EvaluateGlobalTransform(Time) * _TransformMat;
			FbxAMatrix CurMat = OffsetMat.Inverse() * _Cluster->GetLink()->EvaluateGlobalTransform(Time);

			CurMat = ReflectMat * CurMat * ReflectMat;

			FbxKeyFrame KeyFrame = {};

			KeyFrame.Time = Time.GetSecondDouble();
			KeyFrame.TransformMat = CurMat;

			ClipVec[AA]->BoneKeyFrameVec[_BoneIndex].KeyFrameVec.push_back(KeyFrame);
		}
	}
}

void FbxLoader::ChangeWeightAndIndices(Ezptr<FbxMeshContainer> _Container)
{
	std::unordered_map<int, std::vector<FbxWeight>>::iterator StartIter = _Container->WeightMap.begin();
	std::unordered_map<int, std::vector<FbxWeight>>::iterator EndIter = _Container->WeightMap.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		if (StartIter->second.size() > 4)
		{
			sort(StartIter->second.begin(), StartIter->second.end(), [](const FbxWeight& _lhs, const FbxWeight& _rhs)
			{
				return _lhs.Weight > _rhs.Weight;
			});

			double Sum = 0.0;

			for (int i = 0; i < 4; i++)
			{
				Sum += StartIter->second[i].Weight;
			}

			// 보간
			double Interpolate = 1.0 - Sum;

			std::vector<FbxWeight>::iterator EraseIter = StartIter->second.begin() + 4;

			StartIter->second.erase(EraseIter, StartIter->second.end());
			StartIter->second[0].Weight += Interpolate;
		}

		float Weight[4] = {};
		int Index[4] = {};

		for (int i = 0; i < StartIter->second.size(); i++)
		{
			Weight[i] = (float)StartIter->second[i].Weight;
			Index[i] = StartIter->second[i].Index;
		}

		Vec4 WeightVec = Weight;
		Vec4 IndexVec = Index;

		_Container->BlendWeightVec[StartIter->first] = WeightVec;
		_Container->BlendIndexVec[StartIter->first] = IndexVec;
	}
}
