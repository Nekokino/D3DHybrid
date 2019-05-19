#pragma once



#include "EngineHeader.h"
#include "fbxsdk.h"

#ifdef _DEBUG
	#ifdef _M_IX86
		#pragma comment(lib, "libfbxsdk-md_D32")
	#else
		#pragma comment(lib, "libfbxsdk-md_D64")
	#endif
#else
	#ifdef _M_IX86
		#pragma comment(lib, "libfbxsdk-md_R32")
	#else
		#pragma comment(lib, "libfbxsdk-md_R64")
	#endif
#endif

#include "RefCounter.h"

class Engine_DLL FbxBone : public RefCounter
{
public:
	std::string Name;
	int Depth = 0;
	int ParentIndex = 0;
	FbxAMatrix OffsetMat;
	FbxAMatrix BoneMat;
};

class Engine_DLL FbxKeyFrame : public RefCounter
{
public:
	FbxAMatrix TransformMat;
	double Time = 0;
};

class Engine_DLL FbxBoneKeyFrame : public RefCounter
{
public:
	int BoneIndex = 0;
	std::vector<FbxKeyFrame> KeyFrameVec;
};

class Engine_DLL FbxAnimationClip : public RefCounter
{
public:
	std::string Name;
	FbxTime Start;
	FbxTime End;
	FbxLongLong TimeLength;
	FbxTime::EMode TimeMode;
	std::vector<FbxBoneKeyFrame> BoneKeyFrameVec;
};

class Engine_DLL FbxWeight : public RefCounter
{
public:
	int Index = 0;
	double Weight = 0;
};

class Engine_DLL FbxMaterial : public RefCounter
{
public:
	Vec4 Dif;
	Vec4 Amb;
	Vec4 Spc;
	Vec4 Emv;
	float SpcPow = 1.0f;
	float TransparencyFactor = 0.0f;
	float Shininess = 0.0f;
	std::string DifTex;
	std::string BumpTex;
	std::string SpcTex;
};

class Engine_DLL FbxMeshContainer : public RefCounter
{
public:
	std::vector<Vec3> PosVec;
	std::vector<Vec3> NormalVec;
	std::vector<Vec2> UVVec;
	std::vector<Vec3> TangentVec;
	std::vector<Vec3> BinormalVec;
	std::vector<Vec4> BlendWeightVec;
	std::vector<Vec4> BlendIndexVec;
	std::vector<std::vector<UINT>> IndicesVec;
	std::unordered_map<int, std::vector<FbxWeight>> WeightMap;

	bool Bump = false;
	bool Animation = false;
};

enum Engine_DLL FbxLoadType
{
	FLT_MESH,
	FLT_ALL,
};


class Engine_DLL FbxLoader
{
	friend class AIMMesh;
	friend class AIMAnimation;

private:
	FbxManager* Manager = nullptr;
	FbxScene* Scene = nullptr;

private:
	std::vector<std::vector<Ezptr<FbxMaterial>>> MaterialVec;
	std::vector<Ezptr<FbxMeshContainer>> MeshContainerVec;
	std::vector<Ezptr<FbxBone>> BoneVec;
	FbxArray<FbxString*> NameArray;
	std::vector<Ezptr<FbxAnimationClip>> ClipVec;
	bool Mixamo = false;
	int LoadType = FLT_ALL;

public:
	const std::vector<Ezptr<FbxMeshContainer>>* GetMeshContainers() const;
	const std::vector<std::vector<Ezptr<FbxMaterial>>>* GetMaterials() const;
	const std::vector<Ezptr<FbxBone>>* GetBoneVec() const;
	const std::vector<Ezptr<FbxAnimationClip>>* GetClipVec() const;

public:
	bool LoadFbx(const char* _FullPath, FbxLoadType _Type = FLT_ALL);

private:
	void Triangulate(FbxNode* _Node);
	void LoadMaterial(FbxSurfaceMaterial* _Mrtl);
	Vec4 GetMaterialColor(FbxSurfaceMaterial* _Mtrl, const char* _PropertyName, const char* _PropertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial* _Mtrl, const char* _PropertyName);
	std::string GetMaterialTexture(FbxSurfaceMaterial* _Mtrl, const char* _PropertyName);
	void LoadMesh(FbxNode* _Node);
	void LoadMesh(FbxMesh* _Mesh);

	void LoadNormal(FbxMesh* _Mesh, Ezptr<FbxMeshContainer> _Container, int _VtxID, int _ControlIndex);
	void LoadUV(FbxMesh* _Mesh, Ezptr<FbxMeshContainer> _Container, int _UVID, int _ControlIndex);
	void LoadTangent(FbxMesh* _Mesh, Ezptr<FbxMeshContainer> _Container, int _VtxID, int _ControlIndex);
	void LoadBinormal(FbxMesh* _Mesh, Ezptr<FbxMeshContainer> _Container, int _VtxID, int _ControlIndex);

private:
	void LoadAnimationClip();
	void LoadBone(FbxNode* _Node);
	// Recursive = Àç±Í
	void LoadBoneRecursive(FbxNode* _Node, int _Depth, int _Index, int _Parent);
	void LoadAnimation(FbxMesh* _Mesh, Ezptr<FbxMeshContainer> _Container);
	FbxAMatrix GetTransform(FbxNode* _Node);
	int FindBoneFromName(const std::string& _Name);
	void LoadWeightAndIndex(FbxCluster* _Cluster, int _BoneIndex, Ezptr<FbxMeshContainer> _Container);
	void LoadOffsetMatrix(FbxCluster* _Cluster, const FbxAMatrix& _TransformMat, int _BoneIndex, Ezptr<FbxMeshContainer> _Container);
	void LoadTimeTransform(FbxNode* _Node, FbxCluster* _Cluster, const FbxAMatrix& _TransformMat, int _BoneIndex);
	void ChangeWeightAndIndices(Ezptr<FbxMeshContainer> _Container);
private:
	FbxLoader();
	~FbxLoader();
};

