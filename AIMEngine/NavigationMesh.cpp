#include "NavigationMesh.h"


NavigationMesh::NavigationMesh()
{
	Section = new NaviSection[SectionX * SectionZ];

	OpenList.SetSortFunc(this, &NavigationMesh::OpenListSort);
}


NavigationMesh::~NavigationMesh()
{
	if (UseCellList != nullptr)
	{
		delete[] UseCellList;
		UseCellList = nullptr;
	}

	if (Section != nullptr)
	{
		delete[] Section;
		Section = nullptr;
	}

	for (size_t i = 0; i < CellVec.size(); i++)
	{
		if (CellVec[i] != nullptr)
		{
			delete CellVec[i];
			CellVec[i] = nullptr;
		}
	}
}


std::list<Vec3> NavigationMesh::GetPathList()
{
	return PathList;
}

void NavigationMesh::SetOffset(const Vec3 & _Offset)
{
	Offset = _Offset;
}

void NavigationMesh::SetOffsetScale(const Vec3 _OffsetScale)
{
	OffsetScale = _OffsetScale;
}

bool NavigationMesh::Init()
{
	Min.x = 10000000.0f;
	Min.y = 10000000.0f;
	Min.z = 10000000.0f;

	Max.y = -10000000.0f;
	Max.z = -10000000.0f;
	Max.x = -10000000.0f;

	return true;
}


void NavigationMesh::AddCell(const Vec3 _Pos[3])
{
	NavigationCell* Cell = new NavigationCell;

	for (int i = 0; i < 3; ++i)
	{
		Cell->Pos[i] = _Pos[i];
	}

	Cell->Edge[0] = _Pos[1] - _Pos[0];
	Cell->EdgeCenter[0] = (_Pos[1] + _Pos[0]) / 2.f;

	Cell->Edge[1] = _Pos[2] - _Pos[1];
	Cell->EdgeCenter[1] = (_Pos[2] + _Pos[1]) / 2.f;

	Cell->Edge[2] = _Pos[2] - _Pos[0];
	Cell->EdgeCenter[2] = (_Pos[2] + _Pos[0]) / 2.f;

	Cell->Center = (_Pos[0] + _Pos[1] + _Pos[2]) / 3.f;

	Cell->Index = CellVec.size();

	for (int i = 0; i < 3; ++i)
	{
		if (Min.x > _Pos[i].x)
			Min.x = _Pos[i].x;
		if (Min.y > _Pos[i].y)
			Min.y = _Pos[i].y;
		if (Min.z > _Pos[i].z)
			Min.z = _Pos[i].z;

		if (Max.x < _Pos[i].x)
			Max.x = _Pos[i].x;
		if (Max.y < _Pos[i].y)
			Max.y = _Pos[i].y;
		if (Max.z < _Pos[i].z)
			Max.z = _Pos[i].z;
	}

	Vec3 Dir1 = Cell->Edge[0];
	Vec3 Dir2 = Cell->Edge[2];

	Dir1.Normalize();
	Dir2.Normalize();

	Vec3 FaceNormal = Dir1.Cross(Dir2);
	FaceNormal.Normalize();

	Vec3 View = Vec3::Axis[AXIS_Y];

	float	Angle = View.Angle(FaceNormal);

	if (Angle >= 50.f)
		Cell->Enable = false;

	CellVec.push_back(Cell);
}

void NavigationMesh::AddAdj(int _CellIdx, int _AdjIndex)
{
	if (_CellIdx < 0 || _CellIdx >= CellVec.size())
	{
		return;
	}

	AdjInfo Info = {};
	Info.Index = _AdjIndex;
	Info.EdgeIndex = 0;
	CellVec[_CellIdx]->AdjVec.push_back(Info);

}

void NavigationMesh::CreateGridMapAdj(int _LineRectCount)
{
	Grid = true;
	LineRectCount = _LineRectCount;

	size_t iSize = CellVec.size();
	AdjInfo tInfo = {};

	for (size_t i = 0; i < iSize; i += 2)
	{
		// 우상단 삼각형 인접정보 구성.
		// 위쪽 사각형, 오른쪽 사각형, 자기자신 사각형의 왼쪽 하단 삼각형이
		// 인접정보 후보가 된다.
		// 위쪽 사각형의 좌하단 삼각형 인덱스를 구한다.
		int	idx = i - _LineRectCount * 2 + 1;
		if (idx >= 0 && idx < CellVec.size())
		{
			tInfo.Index = idx;
			tInfo.EdgeIndex = 0;
			CellVec[i]->AdjVec.push_back(tInfo);
		}

		// 자기자신 사각형의 왼쪽 하단 삼각형 인덱스를 구한다.
		idx = i + 1;
		if (idx >= 0 && idx < CellVec.size())
		{
			tInfo.Index = idx;
			tInfo.EdgeIndex = 2;
			CellVec[i]->AdjVec.push_back(tInfo);
		}

		// 오른쪽 사각형의 왼쪽 하단 삼각형 인덱스를 구한다.
		if ((i / 2) % _LineRectCount != _LineRectCount - 1)
		{
			idx = i + 3;
			tInfo.Index = idx;
			tInfo.EdgeIndex = 1;
			CellVec[i]->AdjVec.push_back(tInfo);
		}

		// 좌하단 삼각형 인접정보 구성.
		// 아래쪽 사각형, 왼쪽 사각형, 자기자신 사각형의 오른쪽 상단 삼각형이
		// 인접정보 후보가 된다.
		// 자기자신 사각형의 우상단 삼각형 인덱스를 구한다.
		tInfo.Index = i;
		tInfo.EdgeIndex = 0;
		CellVec[i + 1]->AdjVec.push_back(tInfo);

		// 왼쪽 사각형의 우상단 삼각형 인덱스를 구한다.
		if ((i / 2) % _LineRectCount != 0)
		{
			tInfo.Index = i - 3;
			tInfo.EdgeIndex = 2;
			CellVec[i + 1]->AdjVec.push_back(tInfo);
		}

		idx = i + (_LineRectCount * 2);
		if (idx < CellVec.size())
		{
			tInfo.Index = idx;
			tInfo.EdgeIndex = 1;
			CellVec[i + 1]->AdjVec.push_back(tInfo);
		}
	}

	CreateSection();
}

void NavigationMesh::CreateAdj()
{
	size_t	iSize = CellVec.size();

	FILE*	pFile = NULL;

	fopen_s(&pFile, "NaviAdj.txt", "wt");

	Vec3	vOrigin[2][3];
	Vec3	vEdgeDir[2][3];
	float	fLength[2][3];

	for (size_t i = 0; i < iSize; ++i)
	{
		Vec3	vSrc[3];
		Vec3	vSrcOrigin[3];
		float	fSrcLength[3] = {};
		for (int k = 0; k < 3; ++k)
		{
			vEdgeDir[0][k] = CellVec[i]->Edge[k];

			fLength[0][k] = vEdgeDir[0][k].Length();

			vEdgeDir[0][k].Normalize();
		}

		vOrigin[0][0] = CellVec[i]->Pos[0];
		vOrigin[0][1] = CellVec[i]->Pos[1];
		vOrigin[0][2] = CellVec[i]->Pos[2];

		for (size_t j = i + 1; j < iSize; ++j)
		{
			// 삼각형의 3개의 엣지들을 서로 내적하여 평행한지 비교한다.
			Vec3	vDest[3];
			float fDestLength[3];
			Vec3	vDestOrigin[3];
			int	iPosIdx[3][2] = { 1, 0, 2, 1, 2, 0 };
			bool	bInsert = false;

			vOrigin[1][0] = CellVec[j]->Pos[0];
			vOrigin[1][1] = CellVec[j]->Pos[1];
			vOrigin[1][2] = CellVec[j]->Pos[2];

			for (int k = 0; k < 3; ++k)
			{
				vEdgeDir[1][k] = CellVec[j]->Edge[k];

				fLength[1][k] = vEdgeDir[1][k].Length();

				vEdgeDir[1][k].Normalize();
			}

			for (int k = 0; k < 3; ++k)
			{
				if (bInsert)
					break;

				for (int l = 0; l < 3; ++l)
				{
					// A삼각형과 B삼각형의 엣지를 구성하는 두 정점이 같을 경우
					// 이 삼각형은 무조건 붙어있는 삼각형이다.
					if ((vOrigin[0][iPosIdx[k][0]] == vOrigin[1][iPosIdx[l][0]] ||
						vOrigin[0][iPosIdx[k][0]] == vOrigin[1][iPosIdx[l][1]]) &&
						(vOrigin[0][iPosIdx[k][1]] == vOrigin[1][iPosIdx[l][0]] ||
							vOrigin[0][iPosIdx[k][1]] == vOrigin[1][iPosIdx[l][1]]))
					{
						AdjInfo	tInfo = {};
						tInfo.Index = j;
						tInfo.EdgeIndex = k;
						CellVec[i]->AdjVec.push_back(tInfo);

						tInfo.Index = i;
						tInfo.EdgeIndex = l;
						CellVec[j]->AdjVec.push_back(tInfo);
						bInsert = true;
						break;
					}

					// 두 엣지에 대해서 상대 엣지를 구성하는 2개의 점이 대상 엣지
					// 위에 둘다 존재하는지를 판단한다. 둘다 존재한다면
					// 무조건 붙어있는 것이다.
					// (B1 - A1, B2 - A1), (B1 - A2, B2 - A2)
					// 내적을 통해서 빼준 벡터의 크기가 그대로 나온다면
					// 직선상에 존재하는 것이다. 단, 둘다 크기가 나올 경우는
					// 무조건 존재하는 것이고 둘중 하나만 나온다면 크기가 0이라면
					// 하나의 점만 직선위에 존재하고 두 점 위치가 같다는 것이므로
					// 해당 삼각형은 인접 삼각형이 아니다.
					// B1 - A1 처리
					if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][0]],
						vOrigin[0][iPosIdx[k][0]], vEdgeDir[0][k], fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B2 - A1 처리
					else if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][1]],
						vOrigin[0][iPosIdx[k][0]], vEdgeDir[0][k], fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B1 - A2 처리
					else if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][0]],
						vOrigin[0][iPosIdx[k][1]], vEdgeDir[0][k] * -1.f, fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B2 - A2 처리
					else if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][1]],
						vOrigin[0][iPosIdx[k][1]], vEdgeDir[0][k] * -1.f, fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}
				}
			}
		}
	}

	fclose(pFile);

	CreateSection();
}

bool NavigationMesh::CheckOnEdge(int _Src, int _Dest, const Vec3 & _Origin1, const Vec3 & _Origin2, const Vec3 & _Edge, float _EdgeLength, int _Edge1, int _Edge2)
{
	if ((int)(_Origin2.x * 100) == (int)(_Origin1.x * 100) && (int)(_Origin2.y * 100) == (int)(_Origin1.y * 100) &&
		(int)(_Origin2.z * 100) == (int)(_Origin1.z * 100))
		return false;

	Vec3	vResult = _Origin1 - _Origin2;

	float	fDist = vResult.Length();

	// 위에서 구해준 거리를 이용해서 원점으로부터 엣지 방향으로 지정된 거리만큼 이동된 위치에 대상 점이 있는지를
	// 판단한다.
	vResult = _Origin2 + _Edge * fDist;

	if ((int)(vResult.x * 100) == (int)(_Origin1.x * 100) && (int)(vResult.y * 100) == (int)(_Origin1.y * 100) &&
		(int)(vResult.z * 100) == (int)(_Origin1.z * 100))
	{
		vResult -= _Origin2;
		if (vResult.Length() < _EdgeLength)
		{
			AdjInfo	tInfo = {};
			tInfo.Index = _Dest;
			tInfo.EdgeIndex = _Edge1;
			CellVec[_Src]->AdjVec.push_back(tInfo);

			tInfo.Index = _Src;
			tInfo.EdgeIndex = _Edge2;
			CellVec[_Dest]->AdjVec.push_back(tInfo);
			return true;
		}
	}

	return false;
}

void NavigationMesh::CheckPathDir(const Vec3 & _Src1, const Vec3 & _Src2, const Vec3 & _Dest1, const Vec3 & _Dest2, Vec3 & _Intersect)
{
}

void NavigationMesh::FindPath(const Vec3 & _Start, const Vec3 & _End)
{
	NavigationCell* Start = FindCell(_Start);
	NavigationCell* End = FindCell(_End);

	if (Start == nullptr || End == nullptr)
	{
		return;
	}

	else if (End->Enable == false)
	{
		return;
	}

	if (Start == End)
	{
		PathList.clear();
		PathList.push_back(_End);

		return;
	}

	if (UseCellList == nullptr)
	{
		UseCellList = new NavigationCell*[CellVec.size()];

	}

	for (int i = 0; i < UseCellListSize; i++)
	{
		UseCellList[i]->Type = NCLT_NONE;
		UseCellList[i]->ParentIndex = -1;
		UseCellList[i]->G = -1.0f;
		UseCellList[i]->H = -1.0f;
		UseCellList[i]->Total = -1.0f;
	}

	UseCellListSize = 0;

	while (FindStack.empty() == false)
	{
		FindStack.pop();
	}

	OpenList.Clear();

	Start->Type = NCLT_OPEN;
	Start->G = 0.0f;
	Start->H = _Start.Distance(_End);
	Start->Total = Start->H;

	OpenList.Insert(Start);

	FindEnd = false;

	NavigationCell* Cell = nullptr;

	while (false == OpenList.Empty() && FindEnd == false)
	{
		OpenList.Pop(Cell);

		Cell->Type = NCLT_CLOSE;

		AddOpenList(Cell, End, _Start, _End);
	}
}

float NavigationMesh::GetY(const Vec3 & _Pos)
{
	if (Grid == false)
	{
		int	iSectionIndex = GetSectionIndex(_Pos);

		if (0 <= iSectionIndex && iSectionIndex < SectionX * SectionZ)
		{
			Vec3	vOrigin = _Pos;
			vOrigin.y = Max.y;
			Vec3	vDir = Vec3(0.f, -1.f, 0.f);

			for (int i = 0; i < Section[iSectionIndex].CL.Size; ++i)
			{
				Vec3	vIntersect;
				float	fDist = 0.f;
				PNavigationCell	pCell = Section[iSectionIndex].CL.CellList[i];

				if (RayIntersectTriangle(vOrigin, vDir, pCell->Pos[0],
					pCell->Pos[1],
					pCell->Pos[2],
					fDist, vIntersect))
					return vIntersect.y;
			}
		}

		return 0.f;
	}

	Vec3	vCellSize = (Max - Min) / LineRectCount;
	Vec3	vConvertPos = _Pos - Min;

	// 가로, 세로를 1로 만들어준다.
	vConvertPos.x /= vCellSize.x;
	vConvertPos.z /= vCellSize.z;

	// 사각형 인덱스를 구한다.
	int	idxX = (int)vConvertPos.x;
	int	idxZ = LineRectCount - ((int)vConvertPos.z + 1);

	if (idxX < 0 || idxX >= LineRectCount)
		return 0.f;

	else if (idxZ < 0 || idxZ >= LineRectCount)
		return 0.f;

	int	idx = (idxZ * LineRectCount + idxX) * 2;

	if (!CellVec[idx]->Enable)
		return 0;

	// 구해준 사각형의 좌상단 점을 구한다.
	Vec3	vLTPos = CellVec[idx]->Pos[0];
	vLTPos.x /= vCellSize.x;
	vLTPos.z /= vCellSize.z;

	float	fX = vConvertPos.x - vLTPos.x;
	float	fZ = vLTPos.z - vConvertPos.z;

	// 우상단 삼각형일 경우
	if (fX >= fZ)
	{
		// Cell을 구성하는 점의 Y값을 얻어온다.
		float	fY[3] = {};
		for (int i = 0; i < 3; ++i)
		{
			fY[i] = CellVec[idx]->Pos[i].y;
		}

		return fY[0] + (fY[1] - fY[0]) * fX + (fY[2] - fY[1]) * fZ;
	}

	// 좌하단 삼각형일 경우
	float	fY[3] = {};
	for (int i = 0; i < 3; ++i)
	{
		fY[i] = CellVec[idx + 1]->Pos[i].y;
	}

	return fY[0] + (fY[1] - fY[2]) * fX + (fY[2] - fY[0]) * fZ;
}

bool NavigationMesh::CheckCell(const Vec3 & _Pos)
{
	int CellIndex = GetCellIndex(_Pos);

	if (CellIndex == -1)
	{
		return false;
	}

	float Y = GetY(CellIndex, _Pos);

	if (false == CellVec[CellIndex]->Enable || (_Pos.y - 2.0f > Y || Y > _Pos.y + 2.0f))
	{
		return false;
	}

	return true;
}

bool NavigationMesh::GetCellEnable(const Vec3 & _Pos)
{
	int CellIndex = GetCellIndex(_Pos);

	if (CellIndex == -1)
	{
		return false;
	}

	return CellVec[CellIndex]->Enable;
}

float NavigationMesh::GetY(int _CellIndex, const Vec3 & _Pos)
{
	return 0.0f;
}

void NavigationMesh::CreateSection()
{
	SectionX = 5;
	SectionZ = 5;

	Vec3 Length = Max - Min;
	SectionSize = Length / Vec3(SectionX, 1, SectionZ);

	if (Section != nullptr)
	{
		delete[] Section;
		Section = nullptr;
	}

	Section = new NaviSection[SectionX * SectionZ];

	for (int AA = 0; AA < SectionZ; AA++)
	{
		for (int BB = 0; BB < SectionX; BB++)
		{
			int idx = AA * SectionX + BB;
			Section[idx].Size = SectionSize;
			Section[idx].Min = Min + SectionSize *Vec3(BB, 0, AA);
			Section[idx].Max = Min + SectionSize * Vec3((BB + 1), 1, (AA + 1));
		}
	}

	for (size_t AA = 0; AA < CellVec.size(); AA++)
	{
		int MinX = 100000, MinZ = 100000;
		int MaxX = -100000, MaxZ = -100000;

		for (int BB = 0; BB < 3; BB++)
		{
			Vec3 Pos = CellVec[AA]->Pos[BB] - Min;
			Pos /= SectionSize;

			int x, z;
			x = (int)Pos.x;
			z = (int)Pos.z;

			if (MinX > x)
			{
				MinX = x;
			}

			if (MinZ > z)
			{
				MinZ = z;
			}

			if (MaxX < x)
			{
				MaxX = x;
			}

			if (MaxZ < z)
			{
				MaxZ = z;
			}
		}

		MinX = MinX < 0 ? 0 : MinX;
		MinZ = MinZ < 0 ? 0 : MinZ;
		MaxX = MaxX >= SectionX ? SectionX - 1 : MaxX;
		MaxZ = MaxZ >= SectionZ ? SectionZ - 1 : MaxZ;

		if (MinX < SectionX && MaxX >= 0 && MinZ < SectionZ && MaxX >= 0)
		{
			for (int BB = MinZ; BB <= MaxZ; BB++)
			{
				for (int CC = MinX; CC <= MaxX; CC++)
				{
					int idx = BB * SectionX + CC;
					Section[BB].Add(CellVec[AA]);
				}
			}
		}

	}
}

void NavigationMesh::AddOpenList(NavigationCell * _Cell, NavigationCell * _CellEnd, const Vec3 & _Start, const Vec3 & _End)
{
	for (size_t AA = 0; AA < _Cell->AdjVec.size(); AA++)
	{
		if (_Cell->AdjVec[AA].Index == -1)
		{
			continue;
		}

		NavigationCell* Adj = CellVec[_Cell->AdjVec[AA].Index];

		if (Adj->Enable == false)
		{
			continue;
		}

		else if (Adj->Type == NCLT_CLOSE)
		{
			continue;
		}

		if (Adj == _CellEnd)
		{
			int Parent = _Cell->Index;

			while (Parent != -1)
			{
				FindStack.push(Parent);
				Parent = CellVec[Parent]->ParentIndex;
			}

			FindStack.pop();

			std::vector<int> PathIndexVec;

			while (FindStack.empty() == false)
			{
				int idx = FindStack.top();
				FindStack.pop();
				PathIndexVec.push_back(idx);
			}

			PathList.clear();

			std::vector<int> CenterVec;

			for (size_t BB = 0; BB < PathIndexVec.size() - 1; BB++)
			{
				int EdgeIndex = -1;

				_Cell = CellVec[PathIndexVec[BB]];

				for (size_t CC = 0; CC < _Cell->AdjVec.size(); CC++)
				{
					if (_Cell->AdjVec[CC].Index == PathIndexVec[BB + 1])
					{
						EdgeIndex = _Cell->AdjVec[CC].EdgeIndex;
						break;
					}
				}

				CenterVec.push_back(EdgeIndex);

				PathList.push_back(_Cell->EdgeCenter[EdgeIndex]);
			}

			PathList.push_back(_End);

			FindEnd = true;

			return;
		}

		float G = Adj->Center.Distance(_Cell->Center);
		float H = Adj->Center.Distance(_End);

		if (Adj->Type == NCLT_NONE)
		{
			UseCellList[UseCellListSize] = Adj;
			++UseCellListSize;

			Adj->G = G + _Cell->G;
			Adj->H = H;
			Adj->Total = Adj->G + Adj->H;
			Adj->ParentIndex = _Cell->Index;
			Adj->Type = NCLT_OPEN;

			OpenList.Insert(Adj);
		}
		else if (Adj->G > G + _Cell->G)
		{
			Adj->G = G + _Cell->G;
			Adj->H = H;
			Adj->Total = Adj->G + Adj->H;
			Adj->ParentIndex = _Cell->Index;
			OpenList.Sort();
		}
	}
}

NavigationCell * NavigationMesh::FindCell(const Vec3 & _Pos)
{
	int Index = GetCellIndex(_Pos);

	if (Index < 0 || Index >= CellVec.size())
	{
		return nullptr;
	}

	return CellVec[Index];
}

int NavigationMesh::GetCellIndex(const Vec3 & _Pos)
{
	if (Grid == false)
	{
		int SectionIndex = GetSectionIndex(_Pos);

		if (0 <= SectionIndex && SectionIndex < SectionX * SectionZ)
		{
			Vec3 Origin = _Pos;
			Origin.y = Max.y;
			Vec3 Dir = Vec3(0.0f, -1.0f, 0.0f);

			for (int i = 0; i < Section[SectionIndex].CL.Size; i++)
			{
				Vec3 Intersect;
				float Dist = 0.0f;
				NavigationCell* Cell = Section[SectionIndex].CL.CellList[i];

				if (RayIntersectTriangle(Origin, Dir, Cell->Pos[0], Cell->Pos[1], Cell->Pos[2], Dist, Intersect) == true)
				{
					return Cell->Index;
				}
			}
		}
		return -1;
	}

	Vec3 CellSize = (Max - Min) / LineRectCount;
	Vec3 ConvertPos = _Pos - Min;

	ConvertPos.x /= CellSize.x;
	ConvertPos.z /= CellSize.z;

	int IndexX = (int)ConvertPos.x;
	int IndexZ = LineRectCount - ((int)ConvertPos.z + 1);

	if (IndexX < 0 || IndexX > LineRectCount)
	{
		return -1;
	}
	else if (IndexZ < 0 || IndexZ > LineRectCount)
	{
		return -1;
	}

	return (IndexZ * LineRectCount + IndexX) * 2;
}

bool NavigationMesh::RayIntersectTriangle(Vec3 _RayOrigin, Vec3 _RayDir, Vec3 _0, Vec3 _1, Vec3 _2, float & _T, Vec3& _Intersect)
{
	Vec3 e1, e2, h, s, q;
	float a, f, u, v;

	e1 = _1 - _0;
	e2 = _2 - _0;
	h = _RayDir.Cross(e2);
	a = e1.Dot(h);

	if (a > -0.00001 && a < 0.00001)
		return false;

	f = 1.f / a;
	s = _RayOrigin - _0;

	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f)
		return false;

	q = s.Cross(e1);

	v = f * _RayDir.Dot(q);

	if (v < 0.f || u + v > 1.f)
		return false;

	_T = f * e2.Dot(q);

	if (_T > 0.00001)
	{
		_Intersect = _RayOrigin + _RayDir * _T;
		return true;
	}

	return false;
}

int NavigationMesh::GetSectionIndex(Vec3 _Pos)
{
	_Pos -= Min;
	_Pos /= SectionSize;

	int x, z;
	x = (int)_Pos.x;
	z = (int)_Pos.z;

	return z * SectionX + x;
}

void NavigationMesh::Save(const char * _FileName, const std::string & _Path)
{
}

void NavigationMesh::SaveFromFullPath(const char * _FullPath)
{
}

void NavigationMesh::Load(const char * _FileName, const std::string & _Path)
{
}

void NavigationMesh::LoadFromFullPath(const char * _FullPath)
{
}

bool NavigationMesh::OpenListSort(const PNavigationCell& _Src, const PNavigationCell& _Dest)
{
	return _Src->Total > _Dest->Total;
}