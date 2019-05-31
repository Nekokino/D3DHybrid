#include "NavigationManager.h"

std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>> NavigationManager::NaviMeshMap;


bool NavigationManager::Init()
{
	return true;
}

void NavigationManager::Release()
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::iterator StartIter = NaviMeshMap.begin();
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::iterator EndIter = NaviMeshMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		StartIter->second.clear();
	}

	NaviMeshMap.clear();
}

Ezptr<NavigationMesh> NavigationManager::CreateNaviMesh(AIMScene* _Scene, const std::string & _Name)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::iterator FindIter = NaviMeshMap.find(_Scene);

	std::unordered_map<std::string, Ezptr<NavigationMesh>>* MapPointer;

	if (FindIter == NaviMeshMap.end())
	{
		std::unordered_map<std::string, Ezptr<NavigationMesh>> Map;
		NaviMeshMap.insert(std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::value_type(_Scene, Map));

		MapPointer = &NaviMeshMap.begin()->second;
	}
	else
	{
		MapPointer = &FindIter->second;
	}

	Ezptr<NavigationMesh> Mesh = new NavigationMesh;

	Mesh->SetNameTag(_Name);
	Mesh->Init();

	MapPointer->insert(std::unordered_map<std::string, Ezptr<NavigationMesh>>::value_type(_Name, Mesh));

	return Mesh;
}

Ezptr<NavigationMesh> NavigationManager::CreateNaviMesh(AIMScene* _Scene, const std::string & _Name, const char * _FileName, const std::string & _Path)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::iterator FindIter = NaviMeshMap.find(_Scene);

	std::unordered_map<std::string, Ezptr<NavigationMesh>>* MapPointer;

	if (FindIter == NaviMeshMap.end())
	{
		std::unordered_map<std::string, Ezptr<NavigationMesh>> Map;
		NaviMeshMap.insert(std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::value_type(_Scene, Map));

		MapPointer = &NaviMeshMap.begin()->second;
	}
	else
	{
		MapPointer = &FindIter->second;
	}

	Ezptr<NavigationMesh> Mesh = new NavigationMesh;

	Mesh->SetNameTag(_Name);
	Mesh->Init();
	Mesh->Load(_FileName, _Path);

	MapPointer->insert(std::unordered_map<std::string, Ezptr<NavigationMesh>>::value_type(_Name, Mesh));

	return Mesh;
}

Ezptr<NavigationMesh> NavigationManager::FindNaviMesh(AIMScene* _Scene, const std::string & _Name)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::iterator FindIter = NaviMeshMap.find(_Scene);

	if (FindIter == NaviMeshMap.end())
	{
		return nullptr;
	}

	std::unordered_map<std::string, Ezptr<NavigationMesh>>::iterator FindIter2 = FindIter->second.find(_Name);

	if (FindIter2 == FindIter->second.end())
	{
		return nullptr;
	}

	return FindIter2->second;
}

Ezptr<NavigationMesh> NavigationManager::FindNaviMesh(AIMScene* _Scene, const Vec3 & _Pos)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::iterator FindIter = NaviMeshMap.find(_Scene);

	std::unordered_map<std::string, Ezptr<NavigationMesh>>* MapPointer;

	if (FindIter == NaviMeshMap.end())
	{
		return nullptr;
	}

	std::unordered_map<std::string, Ezptr<NavigationMesh>>::iterator StartIter = FindIter->second.begin();
	std::unordered_map<std::string, Ezptr<NavigationMesh>>::iterator EndIter = FindIter->second.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		Ezptr<NavigationMesh> NaviMesh = StartIter->second;

		Vec3 Min = NaviMesh->GetMin();
		Vec3 Max = NaviMesh->GetMax();

		if (Min.x <= _Pos.x && _Pos.x <= Max.x && Min.y <= _Pos.y && _Pos.y <= Max.y && Min.z <= _Pos.z && _Pos.z <= Max.z)
		{
			return NaviMesh;
		}
	}

	return nullptr;
}

bool NavigationManager::EraseNaviMesh(AIMScene* _Scene, const std::string & _Name)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::iterator FindIter = NaviMeshMap.find(_Scene);

	if (FindIter == NaviMeshMap.end())
	{
		return false;
	}

	std::unordered_map<std::string, Ezptr<NavigationMesh>>::iterator FindIter2 = FindIter->second.find(_Name);

	if (FindIter2 == FindIter->second.end())
	{
		return false;
	}

	FindIter->second.erase(FindIter2);

	return true;
}

bool NavigationManager::EraseNaviMesh(AIMScene* _Scene, const Vec3 & _Pos)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::iterator FindIter = NaviMeshMap.find(_Scene);

	if (FindIter == NaviMeshMap.end())
	{
		return false;
	}

	std::unordered_map<std::string, Ezptr<NavigationMesh>>::iterator StartIter = FindIter->second.begin();
	std::unordered_map<std::string, Ezptr<NavigationMesh>>::iterator EndIter = FindIter->second.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		Ezptr<NavigationMesh> NaviMesh = StartIter->second;

		Vec3 Min = NaviMesh->GetMin();
		Vec3 Max = NaviMesh->GetMax();

		if (Min.x <= _Pos.x && _Pos.x <= Max.x && Min.y <= _Pos.y && _Pos.y <= Max.y && Min.z <= _Pos.z && _Pos.z <= Max.z)
		{
			FindIter->second.erase(StartIter);
			return true;
		}
	}

	return false;
}

bool NavigationManager::EraseNaviMesh(AIMScene* _Scene)
{
	std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>>::iterator FindIter = NaviMeshMap.find(_Scene);

	if (FindIter == NaviMeshMap.end())
	{
		return false;
	}

	FindIter->second.clear();
	NaviMeshMap.erase(FindIter);

	return true;
}
