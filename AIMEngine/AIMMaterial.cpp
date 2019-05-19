#include "AIMMaterial.h"
#include "ResourceManager.h"
#include "AIMTexture.h"
#include "AIMSampler.h"
#include "ShaderManager.h"
#include "PathManager.h"

AIMMaterial::AIMMaterial()
{
	CT = CT_MATERIAL;
}

AIMMaterial::AIMMaterial(const AIMMaterial & _Other) : AIMComponent(_Other)
{
	ContainerVec.clear();

	for (size_t AA = 0; AA < _Other.ContainerVec.size(); ++AA)
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;

		for (size_t BB = 0; BB < _Other.ContainerVec[AA]->SubsetVec.size(); ++BB)
		{
			Ezptr<MaterialSubset> Subset = new MaterialSubset;

			Subset->Mtrl = _Other.ContainerVec[AA]->SubsetVec[BB]->Mtrl;

			std::unordered_map<std::string, Ezptr<TextureSet>>::const_iterator StartIter = _Other.ContainerVec[AA]->SubsetVec[BB]->TextureSetMap.begin();
			std::unordered_map<std::string, Ezptr<TextureSet>>::const_iterator EndIter = _Other.ContainerVec[AA]->SubsetVec[BB]->TextureSetMap.end();

			for (; StartIter != EndIter ; ++StartIter)
			{
				Ezptr<TextureSet> Tmp = new TextureSet;

				*Tmp = *(StartIter->second);

				Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(StartIter->first, Tmp));
			}

			Container->SubsetVec.push_back(Subset);
		}
		ContainerVec.push_back(Container);
	}
}


AIMMaterial::~AIMMaterial()
{
	ContainerVec.clear();
}

void AIMMaterial::SetColor(const Vec4 & _Dif, const Vec4 & _Amb, const Vec4 & _Spc, const Vec4 & _Emv, float _SpcPower, UINT _Container, UINT _Subset)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	memcpy(&Subset->Mtrl.Diffuse, &_Dif, sizeof(Vec4));
	memcpy(&Subset->Mtrl.Ambient, &_Amb, sizeof(Vec4));
	memcpy(&Subset->Mtrl.Emissive, &_Emv, sizeof(Vec4));
	memcpy(&Subset->Mtrl.Specular, &_Spc, sizeof(Vec4));

	Subset->Mtrl.Specular.w = _SpcPower;
}

void AIMMaterial::SetEmissiveColor(float _Emv)
{
	for (size_t AA = 0; AA < ContainerVec.size(); AA++)
	{
		for (size_t BB = 0; BB < ContainerVec[AA]->SubsetVec.size(); BB++)
		{
			ContainerVec[AA]->SubsetVec[BB]->Mtrl.Emissive.x = _Emv;
			ContainerVec[AA]->SubsetVec[BB]->Mtrl.Emissive.y = _Emv;
			ContainerVec[AA]->SubsetVec[BB]->Mtrl.Emissive.z = _Emv;
			ContainerVec[AA]->SubsetVec[BB]->Mtrl.Emissive.w = 1.0f;
		}
	}
}

void AIMMaterial::SetEmissiveColor(int _Container, int _Subset, float _Emv)
{
	ContainerVec[_Container]->SubsetVec[_Subset]->Mtrl.Emissive.x = _Emv;
	ContainerVec[_Container]->SubsetVec[_Subset]->Mtrl.Emissive.y = _Emv;
	ContainerVec[_Container]->SubsetVec[_Subset]->Mtrl.Emissive.z = _Emv;
	ContainerVec[_Container]->SubsetVec[_Subset]->Mtrl.Emissive.w = 1.0f;
}

void AIMMaterial::AddTextureSet(int _Container, int _Subset, int _TexReg, const std::string & _TexName, const TCHAR * _FileName, const std::string & _Path)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	if (FindTextureSet(_Container, _Subset, _TexName) != nullptr)
	{
		return;
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Ezptr<TextureSet> Tmp = new TextureSet;

	if (_FileName != nullptr)
	{
		ResourceManager::LoadTexture(_TexName, _FileName, _Path);
	}

	Tmp->Texture = ResourceManager::FindTexture(_TexName);

	Tmp->TexReg = _TexReg;

	Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(_TexName, Tmp));
}

void AIMMaterial::AddTextureSetFromFullPath(int _Container, int _Subset, int _TexReg, const std::string & _TexName, const TCHAR* _FullPath)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	if (FindTextureSet(_Container, _Subset, _TexName) != nullptr)
	{
		return;
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Ezptr<TextureSet> Tmp = new TextureSet;

	if (_FullPath != nullptr)
	{
		ResourceManager::LoadTextureFromFullPath(_TexName, _FullPath);
	}

	Tmp->Texture = ResourceManager::FindTexture(_TexName);

	Tmp->TexReg = _TexReg;

	Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(_TexName, Tmp));
}

void AIMMaterial::AddTextureSetArray(int _Container, int _Subset, int _TexReg, const std::string & _TexName, const std::vector<TCHAR*>* _FileNameVec, const std::string & _Path)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	if (FindTextureSet(_Container, _Subset, _TexName) != nullptr)
	{
		return;
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Ezptr<TextureSet> Tmp = new TextureSet;

	if (_FileNameVec != nullptr)
	{
		ResourceManager::LoadTexture(_TexName, *_FileNameVec, _Path);
	}

	Tmp->Texture = ResourceManager::FindTexture(_TexName);

	Tmp->TexReg = _TexReg;

	Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(_TexName, Tmp));
}

void AIMMaterial::AddTextureSetArrayFromFullPath(int _Container, int _Subset, int _TexReg, const std::string & _TexName, const std::vector<TCHAR*>* _FullPathVec)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	if (FindTextureSet(_Container, _Subset, _TexName) != nullptr)
	{
		return;
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Ezptr<TextureSet> Tmp = new TextureSet;

	if (_FullPathVec != nullptr)
	{
		ResourceManager::LoadTextureFromFullPath(_TexName, *_FullPathVec);
	}

	Tmp->Texture = ResourceManager::FindTexture(_TexName);

	Tmp->TexReg = _TexReg;

	Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(_TexName, Tmp));
}

void AIMMaterial::DeleteTextureSet(int _Container, int _Subset, const std::string & _Name)
{
	if (_Container >= ContainerVec.size())
	{
		return;
	}

	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		return;
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	std::unordered_map<std::string, Ezptr<TextureSet>>::iterator FindIter = Subset->TextureSetMap.find(_Name);

	if (FindIter == Subset->TextureSetMap.end())
	{
		return;
	}

	Subset->TextureSetMap.erase(FindIter);
}

void AIMMaterial::ChangeTextureSet(int _Container, int _Subset, int _TexReg, const std::string & _Name, const std::string & _ChangeName, const TCHAR* _FileName, const std::string& _Path)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Ezptr<TextureSet> Origin = FindTextureSet(_Container, _Subset, _Name);

	Origin->TexReg = _TexReg;

	if (_FileName != nullptr)
	{
		ResourceManager::LoadTexture(_ChangeName, _FileName, _Path);
	}
	Origin->Texture = ResourceManager::FindTexture(_ChangeName);

	Subset->TextureSetMap.erase(_Name);
	Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(_ChangeName, Origin));
}

void AIMMaterial::ChangeTextureSetFromFullPath(int _Container, int _Subset, int _TexReg, const std::string & _Name, const std::string & _ChangeName, const TCHAR * _FullPath)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Ezptr<TextureSet> Origin = FindTextureSet(_Container, _Subset, _Name);

	Origin->TexReg = _TexReg;

	if (_FullPath != nullptr)
	{
		ResourceManager::LoadTextureFromFullPath(_ChangeName, _FullPath);
	}
	Origin->Texture = ResourceManager::FindTexture(_ChangeName);

	Subset->TextureSetMap.erase(_Name);
	Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(_ChangeName, Origin));
}

void AIMMaterial::ChangeTextureSetArray(int _Container, int _Subset, int _TexReg, const std::string & _Name, const std::string & _ChangeName, const std::vector<TCHAR*>* _FileNameVec, const std::string & _Path)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Ezptr<TextureSet> Origin = FindTextureSet(_Container, _Subset, _Name);

	Origin->TexReg = _TexReg;

	if (_FileNameVec != nullptr)
	{
		ResourceManager::LoadTexture(_ChangeName, *_FileNameVec, _Path);
	}
	Origin->Texture = ResourceManager::FindTexture(_ChangeName);

	Subset->TextureSetMap.erase(_Name);
	Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(_ChangeName, Origin));
}

void AIMMaterial::ChangeTextureSetArrayFromFullPath(int _Container, int _Subset, int _TexReg, const std::string & _Name, const std::string & _ChangeName, const std::vector<TCHAR*>* _FullPathVec)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Ezptr<TextureSet> Origin = FindTextureSet(_Container, _Subset, _Name);

	Origin->TexReg = _TexReg;

	if (_FullPathVec != nullptr)
	{
		ResourceManager::LoadTextureFromFullPath(_ChangeName, *_FullPathVec);
	}
	Origin->Texture = ResourceManager::FindTexture(_ChangeName);

	Subset->TextureSetMap.erase(_Name);
	Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(_ChangeName, Origin));
}

void AIMMaterial::BumpTextureEnable(int _Container, int _Subset)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Subset->Mtrl.IsNormal = 1;
}

void AIMMaterial::SpecularTextureEnable(int _Container, int _Subset)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}
	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Subset->Mtrl.IsSpecular = 1;
}

Ezptr<TextureSet> AIMMaterial::FindTextureSet(int _Container, int _Subset, const std::string & _Name)
{
	if (_Container >= ContainerVec.size())
	{
		return;
	}

	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		return;
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	std::unordered_map<std::string, Ezptr<TextureSet>>::iterator FindIter = Subset->TextureSetMap.find(_Name);

	if (FindIter == Subset->TextureSetMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void AIMMaterial::Skinning(int _Container, int _Subset)
{
	if (_Container >= ContainerVec.size())
	{
		Ezptr<MaterialContainer> Container = new MaterialContainer;
		ContainerVec.push_back(Container);
	}

	if (_Subset >= ContainerVec[_Container]->SubsetVec.size())
	{
		Ezptr<MaterialSubset> Subset = new MaterialSubset;
		ContainerVec[_Container]->SubsetVec.push_back(Subset);
	}

	Ezptr<MaterialContainer> Container = ContainerVec[_Container];
	Ezptr<MaterialSubset> Subset = Container->SubsetVec[_Subset];

	Subset->Mtrl.Skinning = 1;
}

void AIMMaterial::Start()
{
}

bool AIMMaterial::Init()
{
	return true;
}

int AIMMaterial::Input(float _Time)
{
	return 0;
}

int AIMMaterial::Update(float _Time)
{
	return 0;
}

int AIMMaterial::LateUpdate(float _Time)
{
	return 0;
}

int AIMMaterial::Collision(float _Time)
{
	return 0;
}

int AIMMaterial::PrevRender(float _Time)
{
	return 0;
}

int AIMMaterial::Render(float _Time)
{
	return 0;
}

AIMMaterial * AIMMaterial::Clone() const
{
	return new AIMMaterial(*this);
}

void AIMMaterial::SetShader(int _Container, int _Subset)
{
	Ezptr<MaterialSubset> Subset = ContainerVec[_Container]->SubsetVec[_Subset];

	if (Subset == nullptr)
	{
		return;
	}

	ShaderManager::UpdateConstBuffer("Material", &Subset->Mtrl);

	std::unordered_map<std::string, Ezptr<TextureSet>>::iterator StartIter = Subset->TextureSetMap.begin();
	std::unordered_map<std::string, Ezptr<TextureSet>>::iterator EndIter = Subset->TextureSetMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		StartIter->second->Texture->PSSetShader(StartIter->second->TexReg);
	}
}

void AIMMaterial::Save(FILE * _File)
{
	size_t ContainerSize = ContainerVec.size();

	fwrite(&ContainerSize, sizeof(size_t), 1, _File);

	for (size_t AA = 0; AA < ContainerSize; AA++)
	{
		size_t SubsetSize = ContainerVec[AA]->SubsetVec.size();

		fwrite(&SubsetSize, sizeof(size_t), 1, _File);

		for (size_t BB = 0; BB < SubsetSize; BB++)
		{
			Ezptr<MaterialSubset> Subset = ContainerVec[AA]->SubsetVec[BB];

			fwrite(&Subset->Mtrl, sizeof(Material), 1, _File);

			size_t TexCount = Subset->TextureSetMap.size();

			fwrite(&TexCount, sizeof(size_t), 1, _File);

			std::unordered_map<std::string, Ezptr<TextureSet>>::iterator StartIter = Subset->TextureSetMap.begin();
			std::unordered_map<std::string, Ezptr<TextureSet>>::iterator EndIter = Subset->TextureSetMap.end();

			for (; StartIter != EndIter ; ++StartIter)
			{
				SaveTextureSet(_File, StartIter->second);
			}
		}
	}
}

void AIMMaterial::Load(FILE * _File)
{
	ContainerVec.clear();

	size_t ContainerSize = 0;
	fread(&ContainerSize, sizeof(size_t), 1, _File);

	for (size_t AA = 0; AA < ContainerSize; AA++)
	{
		size_t SubsetSize = 0;
		fread(&SubsetSize, sizeof(size_t), 1, _File);

		Ezptr<MaterialContainer> Container = new MaterialContainer;

		ContainerVec.push_back(Container);

		for (size_t BB = 0; BB < SubsetSize; BB++)
		{
			Ezptr<MaterialSubset> Subset = new MaterialSubset;

			Container->SubsetVec.push_back(Subset);

			fread(&Subset->Mtrl, sizeof(Material), 1, _File);

			size_t TexCount = 0;

			fread(&TexCount, sizeof(size_t), 1, _File);

			for (size_t CC = 0; CC < TexCount; CC++)
			{
				Ezptr<TextureSet> TexSet = new TextureSet;
				LoadTextureSet(_File, &TexSet);

				Subset->TextureSetMap.insert(std::unordered_map<std::string, Ezptr<TextureSet>>::value_type(TexSet->Texture->GetNameTag(), TexSet));
			}
		}
	}
}

void AIMMaterial::SaveFromPath(const char * _FileName, const std::string & _Path)
{
}

void AIMMaterial::LoadFromPath(const char * _FileName, const std::string & _Path)
{
}

void AIMMaterial::SaveTextureSet(FILE * _File, Ezptr<TextureSet> _Texture)
{
	bool TexEnable = false;

	if (_Texture != nullptr)
	{
		TexEnable = true;

		fwrite(&TexEnable, sizeof(bool), 1, _File);

		std::string TextureName = _Texture->Texture->GetNameTag();
		size_t NameLength = TextureName.length();

		// 텍스쳐 이름 저장
		fwrite(&NameLength, sizeof(size_t), 1, _File);
		fwrite(TextureName.c_str(), 1, NameLength, _File);

		// 경로리스트
		const std::vector<TCHAR*>* PathList = _Texture->Texture->GetFullPath();

		size_t PathCount = PathList->size();

		// 경로 갯수 저장
		fwrite(&PathCount, sizeof(size_t), 1, _File);

		// 루트패스 가져와서
		const char* RootPath = PathManager::FindPathMultiByte("Root");
		char Root[MAX_PATH] = {};

		int RootLength = strlen(RootPath);

		// 역순으로 찾는다
		for (int i = RootLength - 2; i >= 0; --i)
		{
			// 디렉토리 부분
			if (RootPath[i] == '\\' || RootPath[i] == '/')
			{
				// 해당되는 디렉토리 이름만 복사한다
				memcpy(Root, &RootPath[i + 1], sizeof(char) * (RootLength - (i + 2)));
				break;
			}
		}

		// 길이
		RootLength = strlen(Root);

		// 역순
		char Reverse[MAX_PATH] = {};

		for (int i = 0; i < RootLength; i++)
		{
			Reverse[i] = Root[RootLength - (i + 1)];
		}

		memcpy(Root, Reverse, MAX_PATH);

		_strupr_s(Root);

		std::vector<TCHAR*>::const_iterator StartIter = PathList->begin();
		std::vector<TCHAR*>::const_iterator EndIter = PathList->end();

		for (; StartIter != EndIter ; ++StartIter)
		{
			int PathLength = lstrlen(*StartIter);
			char Path[MAX_PATH] = {};

#ifdef UNICODE
			WideCharToMultiByte(CP_ACP, 0, *StartIter, -1, Path, PathLength, 0, 0);
#else
			strcpy_s(Path, *StartIter);
#endif
			_strupr_s(Path);

			for (int AA = PathLength - 1; AA >= 0; --AA)
			{
				if (Path[AA] == '\\' || Path[AA] == '/')
				{
					bool Enable = true;

					for (int BB = 1; BB < RootLength; BB++)
					{
						if (Path[AA - BB] != Root[BB - 1])
						{
							Enable = false;
							break;
						}
					}

					if (Enable == true)
					{
						char SavePath[MAX_PATH] = {};
						int SaveCount = PathLength - (AA + 1);
						memcpy(SavePath, &Path[AA + 1], sizeof(char) * SaveCount);
						fwrite(&SaveCount, sizeof(int), 1, _File);
						fwrite(SavePath, sizeof(char), SaveCount, _File);
						break;
					}
				}
			}
		}
		fwrite(&_Texture->TexReg, sizeof(int), 1, _File);
	}

	else
	{
		fwrite(&TexEnable, sizeof(bool), 1, _File);
	}
}

void AIMMaterial::LoadTextureSet(FILE * _File, Ezptr<TextureSet>* _Texture)
{
	bool TexEnable = false;

	fread(&TexEnable, sizeof(bool), 1, _File);

	if (TexEnable == true)
	{
		char TexName[256] = {};

		size_t NameLength = 0;

		fread(&NameLength, sizeof(size_t), 1, _File);
		fread(TexName, 1, NameLength, _File);

		size_t PathCount = 0;
		fread(&PathCount, sizeof(size_t), 1, _File);

		if (PathCount == 1)
		{
			char Path[MAX_PATH] = {};

			int SaveCount = 0;

			fread(&SaveCount, sizeof(int), 1, _File);
			fread(Path, sizeof(char), SaveCount, _File);

			TCHAR LoadPath[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, Path, -1, LoadPath, strlen(Path) * 2);
#else
			strcpy_s(LoadPath, Path);
#endif

			*_Texture = new TextureSet;

			ResourceManager::LoadTexture(TexName, LoadPath, "Root");
			(*_Texture)->Texture = ResourceManager::FindTexture(TexName);
		}
		else
		{
			std::vector<const TCHAR*> PathVec;
			for (size_t i = 0; i < PathCount; i++)
			{
				char Path[MAX_PATH] = {};
				
				int SaveCount = 0;

				fread(&SaveCount, sizeof(int), 1, _File);
				fread(Path, sizeof(char), SaveCount, _File);

				TCHAR* LoadPath = new TCHAR[MAX_PATH];
				memset(LoadPath, 0, sizeof(TCHAR) * MAX_PATH);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, Path, -1, LoadPath, strlen(Path) * 2);
#else
				strcpy_s(LoadPath, Path);
#endif

				PathVec.push_back(LoadPath);
			}

			*_Texture = new TextureSet;
		}
		fread(&(*_Texture)->TexReg, sizeof(int), 1, _File);
	}
}
