#include "PathManager.h"

std::unordered_map<std::string, TCHAR*> PathManager::PathMap;
char PathManager::strChange[MAX_PATH] = {};

bool PathManager::Init()
{
	TCHAR* Path = new TCHAR[MAX_PATH];

	memset(Path, 0, sizeof(TCHAR) * MAX_PATH);

	GetModuleFileName(nullptr, Path, MAX_PATH);

	size_t Length = lstrlen(Path) - 1;

	for (size_t i = Length; i >= 0; --i)
	{
		if (Path[i] == '\\' || Path[i] == '/')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i));
			break;
		}
	}

	PathMap.insert(std::unordered_map<std::string, TCHAR*>::value_type("Root", Path));

	CreatePath("Shader", TEXT("Shader\\"), "Root");
	CreatePath("Texture", TEXT("Texture\\"), "Root");
	CreatePath("Mesh", TEXT("Mesh\\"), "Root");
	CreatePath("Sound", TEXT("Sound\\"), "Root");

	return true;
}

void PathManager::Release()
{
	std::unordered_map<std::string, TCHAR*>::iterator StartIter = PathMap.begin();
	std::unordered_map<std::string, TCHAR*>::iterator EndIter = PathMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (StartIter->second != nullptr)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	PathMap.clear();
}

bool PathManager::CreatePath(const std::string & _PathKey, const TCHAR * _AddPath, const std::string & _BaseKey)
{
	// 중복된 패스 키 검색
	if (nullptr != FindPath(_PathKey))
	{
		tassertmsg(true, "Overlapped Pathkey");
		return false;
	}

	TCHAR* Path = new TCHAR[MAX_PATH];

	memset(Path, 0, sizeof(TCHAR) * MAX_PATH);

	const TCHAR* Base = FindPath(_BaseKey);

	// 베이스 패스를 복사한뒤
	if (nullptr != Base)
	{
		lstrcpy(Path, Base);
	}

	// 추가 패스를 더해준다
	lstrcat(Path, _AddPath);

	PathMap.insert(std::unordered_map<std::string, TCHAR*>::value_type(_PathKey, Path));

	return true;

}

TCHAR * PathManager::FindPath(const std::string & _Key)
{
	std::unordered_map<std::string, TCHAR*>::iterator FindIter = PathMap.find(_Key);

	if (FindIter == PathMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

const char * PathManager::FindPathMultiByte(const std::string & _Key)
{
	std::unordered_map<std::string, TCHAR*>::iterator FindIter = PathMap.find(_Key);

	if (FindIter == PathMap.end())
	{
		return nullptr;
	}

#ifdef UNICODE
	memset(strChange, 0, MAX_PATH);
	WideCharToMultiByte(CP_UTF8, 0, FindIter->second, -1, strChange, lstrlen(FindIter->second), nullptr, nullptr);
	return strChange;
#else
	return FindIter->second;
#endif
}
