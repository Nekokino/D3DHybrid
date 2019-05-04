#include "ResourceManager.h"
#include "AIMMesh.h"
#include "AIMTexture.h"
#include "AIMSampler.h"

std::unordered_map<std::string, Ezptr<AIMMesh>> ResourceManager::MeshMap;
std::unordered_map<std::string, Ezptr<AIMTexture>> ResourceManager::TextureMap;
std::unordered_map<std::string, Ezptr<AIMSampler>> ResourceManager::SamplerMap;
std::vector<Vertex3D> ResourceManager::SphereVtxVec;
std::vector<UINT> ResourceManager::SphereIdxVec;


bool ResourceManager::Init()
{
	CreateSphere(1.0f, 5);

	Vec3 PyramidPos[5] =
	{
		Vec3(0.f, 0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.5f),
		Vec3(0.5f, -0.5f, 0.5f),
		Vec3(-0.5f, -0.5f, -0.5f),
		Vec3(0.5f, -0.5f, -0.5f)
	};

	Vec3 PyramidFaceNormal[4];
	Vec3 Edge[2];

	Edge[0] = PyramidPos[3] - PyramidPos[0];
	Edge[1] = PyramidPos[1] - PyramidPos[0];
	Edge[0].Normalize();
	Edge[1].Normalize();
	PyramidFaceNormal[0] = Edge[0].Cross(Edge[1]);
	PyramidFaceNormal[0].Normalize();

	// ¹Ù±ùÂÊ »ï°¢Çü
	Edge[0] = PyramidPos[1] - PyramidPos[0];
	Edge[1] = PyramidPos[2] - PyramidPos[0];
	Edge[0].Normalize();
	Edge[1].Normalize();
	PyramidFaceNormal[1] = Edge[0].Cross(Edge[1]);
	PyramidFaceNormal[1].Normalize();

	// ¿À¸¥ÂÊ »ï°¢Çü
	Edge[0] = PyramidPos[2] - PyramidPos[0];
	Edge[1] = PyramidPos[4] - PyramidPos[0];
	Edge[0].Normalize();
	Edge[1].Normalize();
	PyramidFaceNormal[2] = Edge[0].Cross(Edge[1]);
	PyramidFaceNormal[2].Normalize();

	// ¾ÈÂÊ »ï°¢Çü
	Edge[0] = PyramidPos[4] - PyramidPos[0];
	Edge[1] = PyramidPos[3] - PyramidPos[0];
	Edge[0].Normalize();
	Edge[1].Normalize();
	PyramidFaceNormal[3] = Edge[0].Cross(Edge[1]);
	PyramidFaceNormal[3].Normalize();

	Vec3 PyramidNormal[4];

	PyramidNormal[0] = PyramidFaceNormal[0] + PyramidFaceNormal[1];
	PyramidNormal[0].Normalize();

	PyramidNormal[1] = PyramidFaceNormal[2] + PyramidFaceNormal[1];
	PyramidNormal[1].Normalize();

	PyramidNormal[2] = PyramidFaceNormal[0] + PyramidFaceNormal[3];
	PyramidNormal[2].Normalize();

	PyramidNormal[3] = PyramidFaceNormal[2] + PyramidFaceNormal[3];
	PyramidNormal[3].Normalize();

	VertexColorNormal	Pyramid[9] =
	{
		VertexColorNormal(PyramidPos[0], Vec3(0.f, 1.f, 0.f), Vec4::Green),
		VertexColorNormal(PyramidPos[1], PyramidNormal[0], Vec4::Red),
		VertexColorNormal(PyramidPos[2], PyramidNormal[1], Vec4::Blue),
		VertexColorNormal(PyramidPos[3], PyramidNormal[2], Vec4::White),
		VertexColorNormal(PyramidPos[4], PyramidNormal[3], Vec4::Black),
		VertexColorNormal(PyramidPos[1], Vec3(0.f, -1.f, 0.f), Vec4::Red),
		VertexColorNormal(PyramidPos[2], Vec3(0.f, -1.f, 0.f), Vec4::Blue),
		VertexColorNormal(PyramidPos[3], Vec3(0.f, -1.f, 0.f), Vec4::White),
		VertexColorNormal(PyramidPos[4], Vec3(0.f, -1.f, 0.f), Vec4::Black)
	};

	int	IdxPyramid[18] = { 0, 3, 1, 0, 1, 2, 0, 2, 4, 0, 4, 3, 7, 8, 6, 7, 6, 5 };

	CreateMesh("Pyramid", "StandardColorNormalShader",
		"StandardColorNormalLayout", Vec3::Axis[AXIS_Y],
		sizeof(VertexColorNormal), 9, D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, Pyramid,
		4, 18, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, IdxPyramid);


	ColorVertex TriColor[3]
	{
		ColorVertex(Vec3(0.0f, 0.5f, 0.0f), Vec4::Green),
		ColorVertex(Vec3(0.5f, -0.5f, 0.0f), Vec4::Red),
		ColorVertex(Vec3(-0.5f, -0.5f, 0.0f), Vec4::Blue)
	};

	int Idx[3] = { 0, 1, 2 };

	CreateMesh("ColorTriangle", "StandardColorShader", "StandardColorLayout", Vec3::Axis[AXIS_Y], sizeof(ColorVertex), 3, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, TriColor, 4, 3, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, Idx);

	VertexUV Rectangle[4] =
	{
		VertexUV(Vec3(0.f, 1.f, 0.f), Vec2(0.f, 0.f)),
		VertexUV(Vec3(1.f, 1.f, 0.f), Vec2(1.f, 0.f)),
		VertexUV(Vec3(0.f, 0.f, 0.f), Vec2(0.f, 1.f)),
		VertexUV(Vec3(1.f, 0.f, 0.f), Vec2(1.f, 1.f)),
	};

	int RectIdx[6] = { 0, 1, 3, 0, 3 ,2 };

	CreateMesh("Rect", "DebugShader", "StandardUVLayout", Vec3::Axis[AXIS_Y], sizeof(VertexUV), 4, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, Rectangle, 4, 6, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, RectIdx);

	CreateSpherePos("Sky", "SkyShader", "StandardPosLayout");

	CreateSampler("LinearSampler");
	CreateSampler("PointSampler", D3D11_FILTER_MIN_MAG_MIP_POINT);

	return true;
}

void ResourceManager::Release()
{
	MeshMap.clear();
	TextureMap.clear();
	SamplerMap.clear();
}

bool ResourceManager::CreateMesh(const std::string & _Name, const std::string& _ShaderName, const std::string& _InputLayoutName, const Vec3& _View, int _VtxSize, int _VtxCount, D3D11_USAGE _VtxUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void * _Vtx, int _IdxSize, int _IdxCount, D3D11_USAGE _IdxUsage, DXGI_FORMAT _Format, void * _Idx)
{
	Ezptr<AIMMesh> Mesh = FindMesh(_Name);

	if (nullptr != Mesh)
	{
		tassertmsg(true, "Can't Create Mesh (Name Overlapped)");
		return false;
	}

	Mesh = new AIMMesh;

	if (false == Mesh->CreateMesh(_Name, _ShaderName, _InputLayoutName, _VtxSize, _VtxCount, _VtxUsage, _Primitive, _Vtx, _IdxSize, _IdxCount, _IdxUsage, _Format, _Idx))
	{
		delete Mesh;
		return false;
	}

	MeshMap.insert(std::unordered_map<std::string, Ezptr<AIMMesh>>::value_type(_Name, Mesh));

	Mesh->SetView(_View);

	return true;
}

bool ResourceManager::CreateSpherePos(const std::string & _Name, const std::string & _Shader, const std::string & _InputLayout)
{
	Ezptr<AIMMesh> Mesh = FindMesh(_Name);

	if (Mesh != nullptr)
	{
		tassertmsg(true, "Overlapped Mesh Name");
		return false;
	}

	Mesh = new AIMMesh;

	std::vector<Vec3> PosVec;
	PosVec.resize(SphereVtxVec.size());

	for (size_t i = 0; i < SphereVtxVec.size(); ++i)
	{
		PosVec[i] = SphereVtxVec[i].Pos;
	}

	if (false == Mesh->CreateMesh(_Name, _Shader, _InputLayout, 12, PosVec.size(), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &PosVec[0]
	, 4, SphereIdxVec.size(), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, &SphereIdxVec[0]))
	{
		return false;
	}

	Mesh->SetView(Vec3(0.0f, 0.0f, 1.0f));

	MeshMap.insert(std::unordered_map<std::string, Ezptr<AIMMesh>>::value_type(_Name, Mesh));

	return true;
}

Ezptr<AIMMesh> ResourceManager::FindMesh(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<AIMMesh>>::iterator iter = MeshMap.find(_Name);

	if (iter == MeshMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool ResourceManager::LoadTexture(const std::string & _Name, const TCHAR * _FileName, const std::string & _Path)
{
	Ezptr<AIMTexture> Tex = FindTexture(_Name);

	if (Tex != nullptr)
	{
		tassertmsg(true, "Overlapped Texture Create");
		return false;
	}

	Tex = new AIMTexture;

	if (Tex->LoadTexture(_Name, _FileName, _Path) == false)
	{
		return false;
	}

	TextureMap.insert(std::unordered_map<std::string, Ezptr<AIMTexture>>::value_type(_Name, Tex));

	return true;
}

Ezptr<AIMTexture> ResourceManager::FindTexture(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<AIMTexture>>::iterator FindIter = TextureMap.find(_Name);

	if (FindIter == TextureMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

bool ResourceManager::CreateSampler(const std::string & _Name, D3D11_FILTER _Filter, D3D11_TEXTURE_ADDRESS_MODE _AddU, D3D11_TEXTURE_ADDRESS_MODE _AddV, D3D11_TEXTURE_ADDRESS_MODE _AddW)
{
	Ezptr<AIMSampler> Smp = FindSampler(_Name);

	if (Smp != nullptr)
	{
		tassertmsg(true, "Overlapped Sampler Name");
		return false;
	}

	Smp = new AIMSampler;

	if (false == Smp->CreateSampler(_Name, _Filter, _AddU, _AddV, _AddW))
	{
		return false;
	}

	SamplerMap.insert(std::unordered_map<std::string, Ezptr<AIMSampler>>::value_type(_Name, Smp));

	return true;
}

Ezptr<AIMSampler> ResourceManager::FindSampler(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<AIMSampler>>::iterator FindIter = SamplerMap.find(_Name);

	if (FindIter == SamplerMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

bool ResourceManager::CreateSphere(float _Rad, unsigned int _SubDiv)
{
	_SubDiv = min(_SubDiv, 5u);

	const float X = 0.525731f;
	const float Z = 0.850651f;

	Vec3 Pos[12] =
	{
		Vec3(-X, 0.0f, Z),  Vec3(X, 0.0f, Z),
		Vec3(-X, 0.0f, -Z), Vec3(X, 0.0f, -Z),
		Vec3(0.0f, Z, X),   Vec3(0.0f, Z, -X),
		Vec3(0.0f, -Z, X),  Vec3(0.0f, -Z, -X),
		Vec3(Z, X, 0.0f),   Vec3(-Z, X, 0.0f),
		Vec3(Z, -X, 0.0f),  Vec3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	SphereVtxVec.resize(12);
	SphereIdxVec.resize(60);

	for (UINT i = 0; i < 12; ++i)
	{
		SphereVtxVec[i].Pos = Pos[i];
	}

	for (UINT i = 0; i < 60; ++i)
	{
		SphereIdxVec[i] = k[i];
	}

	for (UINT i = 0; i < _SubDiv; i++)
	{
		SubDivide();
	}

	for (UINT i = 0; i < SphereVtxVec.size(); ++i)
	{
		Vec3 Normal = SphereVtxVec[i].Pos;
		Normal.Normalize();

		Vec3 Projection = Normal * _Rad;

		SphereVtxVec[i].Pos = Projection;
		SphereVtxVec[i].Normal = Normal;

		float Theta = AngleFromXY(SphereVtxVec[i].Pos.x, SphereVtxVec[i].Pos.z);

		float Phi = acosf(SphereVtxVec[i].Pos.y / _Rad);

		SphereVtxVec[i].UV.x = Theta / DirectX::XM_2PI;
		SphereVtxVec[i].UV.y = Phi / DirectX::XM_PI;

		SphereVtxVec[i].Tangent.x = -_Rad * sinf(Phi) * sinf(Theta);
		SphereVtxVec[i].Tangent.y = 0.0f;
		SphereVtxVec[i].Tangent.z = _Rad * sinf(Phi) * cosf(Theta);

		SphereVtxVec[i].Binormal = SphereVtxVec[i].Normal.Cross(SphereVtxVec[i].Tangent);
	}

	return true;
}

void ResourceManager::SubDivide()
{
	std::vector<Vertex3D> CopyVtx = SphereVtxVec;
	std::vector<UINT> CopyIdx = SphereIdxVec;

	SphereVtxVec.resize(0);
	SphereIdxVec.resize(0);

	UINT numTris = CopyIdx.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		Vertex3D v0 = CopyVtx[CopyIdx[i * 3 + 0]];
		Vertex3D v1 = CopyVtx[CopyIdx[i * 3 + 1]];
		Vertex3D v2 = CopyVtx[CopyIdx[i * 3 + 2]];

		Vertex3D m0, m1, m2;

		m0.Pos = Vec3(
			0.5f*(v0.Pos.x + v1.Pos.x),
			0.5f*(v0.Pos.y + v1.Pos.y),
			0.5f*(v0.Pos.z + v1.Pos.z));

		m1.Pos = Vec3(
			0.5f*(v1.Pos.x + v2.Pos.x),
			0.5f*(v1.Pos.y + v2.Pos.y),
			0.5f*(v1.Pos.z + v2.Pos.z));

		m2.Pos = Vec3(
			0.5f*(v0.Pos.x + v2.Pos.x),
			0.5f*(v0.Pos.y + v2.Pos.y),
			0.5f*(v0.Pos.z + v2.Pos.z));

		SphereVtxVec.push_back(v0);
		SphereVtxVec.push_back(v1);
		SphereVtxVec.push_back(v2);
		SphereVtxVec.push_back(m0);
		SphereVtxVec.push_back(m1);
		SphereVtxVec.push_back(m2);

		SphereIdxVec.push_back(i * 6 + 0);
		SphereIdxVec.push_back(i * 6 + 3);
		SphereIdxVec.push_back(i * 6 + 5);

		SphereIdxVec.push_back(i * 6 + 3);
		SphereIdxVec.push_back(i * 6 + 4);
		SphereIdxVec.push_back(i * 6 + 5);

		SphereIdxVec.push_back(i * 6 + 5);
		SphereIdxVec.push_back(i * 6 + 4);
		SphereIdxVec.push_back(i * 6 + 2);

		SphereIdxVec.push_back(i * 6 + 3);
		SphereIdxVec.push_back(i * 6 + 1);
		SphereIdxVec.push_back(i * 6 + 4);
	}
}

float ResourceManager::AngleFromXY(float _X, float _Y)
{
	float Theta = 0.0f;

	if (_X >= 0.0f)
	{
		Theta = atanf(_Y / _X);

		if (Theta < 0.0f)
		{
			Theta += 2.0f * MATH_PI;
		}
	}

	else
		Theta = atanf(_Y / _X) + MATH_PI;

	return Theta;
}
