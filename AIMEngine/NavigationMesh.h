#pragma once

#include "RefCounter.h"
#include "Heap.h"

enum NavimeshCellListType
{
	NCLT_NONE,
	NCLT_OPEN,
	NCLT_CLOSE,
};

typedef struct Engine_DLL _tagAdjInfo
{
	int Index;
	int EdgeIndex;
}AdjInfo, *PAdjInfo;

typedef struct Engine_DLL _tagNavigationCell
{
	NavimeshCellListType Type;
	Vec3 Pos[3];
	Vec3 Edge[3];
	Vec3 EdgeCenter[3];
	Vec3 Center;
	std::vector<AdjInfo> AdjVec;
	int Index;
	int ParentIndex;
	int G;
	int H;
	int Total;
	bool Enable;

	void Clear()
	{
		Type = NCLT_NONE;
		ParentIndex = -1;
		G = -1.0f;
		H = -1.0f;
		Total = -1.0f;
	}

	_tagNavigationCell()
	{
		Type = NCLT_NONE;
		ParentIndex = -1;
		Index = -1;
		G = -1.0f;
		H = -1.0f;
		Total = -1.0f;
		Enable = true;
	}
}NavigationCell, *PNavigationCell;

typedef struct Engine_DLL _tagCellList
{
	NavigationCell** CellList;
	int Size;
	int Capacity;

	void Resize()
	{
		if (Size == Capacity)
		{
			Capacity *= 2;
			NavigationCell** List = new NavigationCell*[Capacity];

			memset(List, 0, sizeof(NavigationCell*) * Capacity);
			memcpy(List, CellList, sizeof(NavigationCell*) * Size);

			delete[] CellList;
			CellList = List;
		}
	}

	void Add(NavigationCell* _Cell)
	{
		Resize();
		CellList[Size] = _Cell;
		++Size;
	}

	_tagCellList()
	{
		Size = 0;
		Capacity = 2048;
		CellList = new NavigationCell*[Capacity];

		memset(CellList, 0, sizeof(NavigationCell*) * Capacity);
	}

	~_tagCellList()
	{
		if (CellList != nullptr)
		{
			delete[] CellList;
			CellList = nullptr;
		}
	}
}CellList, *PCellList;

typedef struct Engine_DLL _tagNaviSection
{
	Vec3 Size;
	CellList CL;
	Vec3 Min;
	Vec3 Max;

	void Add(NavigationCell* _Cell)
	{
		CL.Add(_Cell);
	}

	_tagNaviSection()
	{

	}

	~_tagNaviSection()
	{

	}
}NaviSection, *PNaviSection;

class Engine_DLL NavigationMesh : public RefCounter
{
	friend class NavigationManager;

private:
	std::string Name;
	std::vector<NavigationCell*> CellVec;
	NavigationCell** UseCellList = nullptr;
	int UseCellListSize = 0;
	Vec3 Offset;
	Vec3 OffsetScale;
	Vec3 Min;
	Vec3 Max;
	int LineRectCount = 0;
	bool Grid = false;
	int SectionX = 1;
	int SectionZ = 1;
	Vec3 SectionSize;
	NaviSection* Section = nullptr;

private:
	AIMHeap<NavigationCell*> OpenList;
	std::stack<int> FindStack;
	std::list<Vec3> PathList;
	bool FindEnd;

public:
	Vec3 GetMin() const
	{
		return Min;
	}

	Vec3 GetMax() const
	{
		return Max;
	}

	Vec3 GetOffset() const
	{
		return Offset;
	}

	std::list<Vec3> GetPathList();

public:
	void SetOffset(const Vec3& _Offset);
	void SetOffsetScale(const Vec3 _OffsetScale);

public:
	bool Init();
	void AddCell(const Vec3 _Pos[3]);
	void AddAdj(int _CellIdx, int _AdjIndex);
	void CreateGridMapAdj(int _LineRectCount);
	void CreateAdj();
	bool CheckOnEdge(int _Src, int _Dest, const Vec3& _Origin1, const Vec3& _Origin2, const Vec3& _Edge, float _EdgeLength, int _Edge1, int _Edge2);
	void CheckPathDir(const Vec3& _Src1, const Vec3& _Src2, const Vec3& _Dest1, const Vec3& _Dest2, Vec3& _Intersect);
	void FindPath(const Vec3& _Start, const Vec3& _End);

public:
	float GetY(const Vec3& _Pos);
	bool CheckCell(const Vec3& _Pos);
	bool GetCellEnable(const Vec3& _Pos);
	float GetY(int _CellIndex, const Vec3& _Pos);
	void CreateSection();

private:
	void AddOpenList(NavigationCell* _Cell, NavigationCell* _CellEnd, const Vec3& _Start, const Vec3& _End);
	NavigationCell* FindCell(const Vec3& _Pos);
	int GetCellIndex(const Vec3& _Pos);
	bool RayIntersectTriangle(Vec3 _RayOrigin, Vec3 RayDir, Vec3 _0, Vec3 _1, Vec3 _2, float& _T, Vec3& _Intersect);
	int GetSectionIndex(Vec3 _Pos);

public:
	void Save(const char* _FileName, const std::string& _Path = "Data");
	void SaveFromFullPath(const char* _FullPath);
	void Load(const char* _FileName, const std::string& _Path = "Data");
	void LoadFromFullPath(const char* _FullPath);

private:
	bool OpenListSort(const PNavigationCell& _Src, const PNavigationCell& _Dest);

private:
	NavigationMesh();
	~NavigationMesh();
};

