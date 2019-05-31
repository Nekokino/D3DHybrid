#pragma once

#include "EngineHeader.h"
#include "RefCounter.h"
#include "AIMScene.h"
#include "NavigationMesh.h"

class Engine_DLL NavigationManager
{

private:
	static std::unordered_map<AIMScene*, std::unordered_map<std::string, Ezptr<NavigationMesh>>> NaviMeshMap;

public:
	static bool Init();
	static void Release();
	static Ezptr<NavigationMesh> CreateNaviMesh(AIMScene*, const std::string& _Name);
	static Ezptr<NavigationMesh> CreateNaviMesh(AIMScene* _Scene, const std::string& _Name, const char* _FileName, const std::string& _Path = "Data");
	static Ezptr<NavigationMesh> FindNaviMesh(AIMScene* _Scene, const std::string& _Name);
	static Ezptr<NavigationMesh> FindNaviMesh(AIMScene* _Scene, const Vec3& _Pos);

	static bool EraseNaviMesh(AIMScene* _Scene, const std::string& _Name);
	static bool EraseNaviMesh(AIMScene* _Scene, const Vec3& _Pos);
	static bool EraseNaviMesh(AIMScene* _Scene);
private:
	NavigationManager() {}
	virtual ~NavigationManager() = 0 {}
};

