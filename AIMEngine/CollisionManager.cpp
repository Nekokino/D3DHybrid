#include "CollisionManager.h"
#include "AIMObject.h"
#include "InputManager.h"
#include "AIMColliderRay.h"
#include "Core.h"
#include "SceneManager.h"
#include <algorithm>

ColliderInfo CollisionManager::ColInfo;
unsigned int CollisionManager::SerialNumber = 0;
CollisionSection* CollisionManager::Section = nullptr;
Vec3 CollisionManager::Pos;
Vec3 CollisionManager::Pivot;
CollisionProfile* CollisionManager::ProfileList = nullptr;
unsigned int CollisionManager::ProfileSize = 0;
unsigned int CollisionManager::ProfileCapacity = 16;
CollisionChannel* CollisionManager::ChannelList = nullptr;
unsigned int CollisionManager::ChannelSize = 0;
unsigned int* CollisionManager::ValidNumber = nullptr;
unsigned int CollisionManager::ValidSize = 0;
unsigned int CollisionManager::ValidCapacity = 100;
Ezptr<AIMCollider> CollisionManager::PrevMousePick;
Ezptr<AIMCollider> CollisionManager::PrevMouseCollider;

CollisionSection* CollisionManager::PickSection;
CollisionSection* CollisionManager::UISection;
ColliderList* CollisionManager::MouseCollisionList;


bool CollisionManager::Init()
{
	ProfileList = new CollisionProfile[ProfileCapacity];
	ChannelList = new CollisionChannel[MAX_COL_CHANNEL];
	ValidNumber = new unsigned int[ValidCapacity];
	memset(ValidNumber, 0, sizeof(unsigned int) * ValidCapacity);

	MouseCollisionList = new ColliderList;

	CreateSection(5, 5, 5, Vec3(30.0f, 30.0f, 30.0f));

	CreateProfile("WorldStatic");
	CreateProfile("WorldDynamic");

	AddChannel("WorldStatic");
	AddChannel("WorldDynamic");

	SetProfileChannelState("WorldStatic", "WorldStatic", CCS_IGNORE);
	SetProfileChannelState("WorldStatic", "WorldDynamic", CCS_BLOCK);

	SetProfileChannelState("WorldDynamic", "WorldStatic", CCS_BLOCK);
	SetProfileChannelState("WorldDynamic", "WorldDynamic", CCS_BLOCK);

	return true;
}

void CollisionManager::Release()
{
	PrevMousePick = nullptr;
	PrevMouseCollider = nullptr;

	if (MouseCollisionList != nullptr)
	{
		delete MouseCollisionList;
		MouseCollisionList = nullptr;
	}

	if (Section != nullptr)
	{
		delete Section;
	}

	if (ProfileList != nullptr)
	{
		delete[] ProfileList;
		ProfileList = nullptr;
	}

	if (ChannelList != nullptr)
	{
		delete[] ChannelList;
		ChannelList = nullptr;
	}

	if (ValidNumber != nullptr)
	{
		delete[] ValidNumber;
		ValidNumber = nullptr;
	}

	if (PickSection != nullptr)
	{
		delete PickSection;
		PickSection = nullptr;
	}

	if (UISection != nullptr)
	{
		delete UISection;
		UISection = nullptr;
	}
}

void CollisionManager::Collision(float _Time)
{
	ComputeSection();

	bool TmpCol = CollisionMouseUI(_Time);

	if (TmpCol == false)
	{
		CollisionMouseWorld(_Time);
	}

	CollisionUI(_Time);
	CollisionWorld(_Time);

	if (PickSection != nullptr)
	{
		for (int i = 0; i < PickSection->NumX * PickSection->NumY * PickSection->NumZ; i++)
		{
			PickSection->SectionList[i].Clear();
		}
	}
}

void CollisionManager::Render(float _Time)
{
	for (unsigned int i = 0; i < ColInfo.Size + ValidSize;)
	{
		AIMCollider* Collider = ColInfo.ColliderList[i];

		if (Collider == nullptr)
		{
			++i;
			continue;
		}

		if (Collider->IsEnable() == false)
		{
			++i;
			continue;
		}
		else if (Collider->IsAlive() == false)
		{
			++i;
			continue;
		}

		Collider->PrevRender(_Time);
		Collider->Render(_Time);
		++i;
	}
}

unsigned int CollisionManager::GetSerialNumber()
{
	if (ValidSize == 0)
	{
		if (SerialNumber == ColInfo.Capacity)
		{
			ColInfo.Resize();
		}
		unsigned int Number = SerialNumber;
		++SerialNumber;
		return Number;
	}

	--ValidSize;

	return ValidNumber[ValidSize];
}

void CollisionManager::AddValidSerialNumber(unsigned int _Number)
{
	if (ValidNumber == nullptr)
	{
		return;
	}

	if (ValidSize == ValidCapacity)
	{
		ValidCapacity *= 2;

		unsigned int* Number = new unsigned int[ValidCapacity];

		memcpy(Number, ValidNumber, sizeof(unsigned int) * ValidSize);

		if (ValidNumber != nullptr)
		{
			delete[] ValidNumber;
			ValidNumber = nullptr;
		}

		ValidNumber = Number;
	}

	ValidNumber[ValidSize] = _Number;
	++ValidSize;

	ColInfo.ColliderList[_Number] = nullptr;
	--ColInfo.Size;
}

void CollisionManager::AddPrevCollider(unsigned int _Src, unsigned int _Dest)
{
	// 32비트를 사용하므로 사전작업

	// 100번이라면
	// Index = 3, Bit = 4가 됨
	unsigned int SrcIndex = _Src / 32;
	unsigned int DestIndex = _Dest / 32;

	// 비트연산을 위해서 비트를 옮겨줌
	unsigned int SrcBit = 31 - (_Src % 32);
	unsigned int DestBit = 31 - (_Dest % 32);

	unsigned int SrcValue = 1 << SrcBit;
	unsigned int DestValue = 1 << DestBit;

	// Src와 Dest의 서로의 위치의 비트를 1로 만들어줌
	ColInfo.AdjMatrix[_Src][DestIndex] |= DestValue;
	ColInfo.AdjMatrix[_Dest][SrcIndex] |= SrcValue;
}

void CollisionManager::DeletePrevCollider(unsigned int _Src, unsigned int _Dest)
{
	unsigned int SrcIndex = _Src / 32;
	unsigned int DestIndex = _Dest / 32;

	unsigned int SrcBit = 31 - (_Src % 32);
	unsigned int DestBit = 31 - (_Dest % 32);

	unsigned int SrcValue = 1 << SrcBit;
	unsigned int DestValue = 1 << DestBit;

	// Src와 Dest 서로의 위치의 비트를 0으로 만들어줌
	if (ColInfo.AdjMatrix[_Src][DestIndex] & DestValue)
	{
		ColInfo.AdjMatrix[_Src][DestIndex] ^= DestValue;
	}

	if (ColInfo.AdjMatrix[_Dest][SrcIndex] & SrcValue)
	{
		ColInfo.AdjMatrix[_Dest][SrcIndex] ^= SrcValue;
	}
}

bool CollisionManager::CheckPrevCollider(unsigned int _Src, unsigned int _Dest)
{
	unsigned int DestIndex = _Dest / 32;

	unsigned int DestBit = 31 - (_Dest % 32);

	unsigned int DestValue = 1 << DestBit;

	if (ColInfo.AdjMatrix[_Src][DestIndex] & DestValue)
	{
		return true;
	}

	return false;
}

void CollisionManager::CreateSection(int _NumX, int _NumY, int _NumZ, const Vec3 & _CellSize, const Vec3 & _Pos, const Vec3 & _Pivot)
{
	if (Section != nullptr)
	{
		delete Section;
		Section = nullptr;
	}

	Section = new CollisionSection;

	Section->NumX = _NumX;
	Section->NumY = _NumY;
	Section->NumZ = _NumZ;

	Section->CellSize = _CellSize;

	Section->SectionList = new ColliderList[_NumX * _NumY * _NumZ];

	Section->Length = Section->CellSize * Vec3(_NumX, _NumY, _NumZ);

	Pos = _Pos;
	Pivot = _Pivot;

	Section->Min = Pos - Pivot * Section->Length;
	Section->Max = Pos + (Vec3(1.0f, 1.0f, 1.0f) - Pivot) * Section->Length;

	if (Core::Inst()->GetEditMode() == true)
	{
		if (PickSection != nullptr)
		{
			delete PickSection;
			PickSection = nullptr;
		}

		PickSection = new CollisionSection;

		PickSection->NumX = _NumX;
		PickSection->NumY = _NumY;
		PickSection->NumZ = _NumZ;
		PickSection->CellSize = _CellSize;

		PickSection->SectionList = new ColliderList[_NumX * _NumY * _NumZ];
		PickSection->Length = PickSection->CellSize * Vec3(_NumX, _NumY, _NumZ);
		PickSection->Min = _Pos - _Pivot * PickSection->Length;
		PickSection->Max = _Pos + (Vec3(1.0f, 1.0f, 1.0f) - _Pivot) * PickSection->Length;
	}

}

bool CollisionManager::AddChannel(const std::string & _Name, CollisionChannelState _State)
{
	if (ChannelSize == MAX_COL_CHANNEL)
	{
		return false;
	}

	ChannelList[ChannelSize].Name = _Name;
	ChannelList[ChannelSize].State = _State;

	for (unsigned int i = 0; i < ProfileSize; i++)
	{
		ProfileList[i].ChannelList[ChannelSize].Name = _Name;
		ProfileList[i].ChannelList[ChannelSize].State = _State;
		++ProfileList[i].ChannelSize;
	}

	++ChannelSize;

	return true;
}

bool CollisionManager::DeleteChannel(const std::string & _Name)
{
	for (unsigned int AA = 0; AA < ChannelSize; AA++)
	{
		if (ChannelList[AA].Name == _Name)
		{
			for (unsigned int BB = AA; BB < ChannelSize; BB++)
			{
				// 채널 리스트를 지울 채널에서부터 한칸씩 당기고
				ChannelList[BB] = ChannelList[BB + 1];
			}

			for (unsigned int BB = 0; BB < ProfileSize; BB++)
			{
				--ProfileList[BB].ChannelSize;
				for (unsigned int CC = AA; CC < ChannelSize; CC++)
				{
					// 모든 채널의 프로필에서 채널 리스트도 한칸씩 당겨준다
					ProfileList[BB].ChannelList[CC] = ProfileList[BB].ChannelList[CC + 1];
				}
			}

			break;
		}
	}

	return true;
}

bool CollisionManager::CreateProfile(const std::string & _Name)
{
	if (ProfileSize == ProfileCapacity)
	{
		ProfileCapacity *= 2;

		CollisionProfile* TmpProfileList = new CollisionProfile[ProfileCapacity];

		for (unsigned int AA = 0; AA < ProfileSize; AA++)
		{
			TmpProfileList[AA].Name = ProfileList[AA].Name;

			for (unsigned int BB = 0; BB < ProfileList[AA].ChannelSize; BB++)
			{
				TmpProfileList[AA].ChannelList[BB] = ProfileList[AA].ChannelList[BB];
			}

			TmpProfileList[AA].ChannelSize = ProfileList[AA].ChannelSize;
		}

		if (ProfileList != nullptr)
		{
			delete[] ProfileList;
			ProfileList = nullptr;
		}

		ProfileList = TmpProfileList;
	}

	ProfileList[ProfileSize].Name = _Name;
	ProfileList[ProfileSize].ChannelSize = ChannelSize;

	for (unsigned int AA = 0; AA < ChannelSize; AA++)
	{
		ProfileList[ProfileSize].ChannelList[AA] = ChannelList[AA];
	}

	++ProfileSize;

	return true;
}

bool CollisionManager::SetProfileChannelState(const std::string & _Name, const std::string & _Channel, CollisionChannelState _State)
{
	for (unsigned int AA = 0; AA < ProfileSize; AA++)
	{
		if (ProfileList[AA].Name == _Name)
		{
			for (unsigned int BB = 0; BB < ChannelSize; BB++)
			{
				if (ProfileList[AA].ChannelList[BB].Name == _Channel)
				{
					ProfileList[AA].ChannelList[BB].State = _State;
					return true;
				}
			}
		}
	}

	return false;
}

CollisionProfile * CollisionManager::FindProfile(const std::string & _Name)
{
	for (unsigned int i = 0; i < ChannelSize; i++)
	{
		if (ProfileList[i].Name == _Name)
		{
			return &ProfileList[i];
		}
	}

	return nullptr;
}

unsigned int CollisionManager::GetChannelIndex(const std::string & _Name)
{
	for (unsigned int i = 0; i < ChannelSize; i++)
	{
		if (ChannelList[i].Name == _Name)
		{
			return i;
		}
	}

	return UINT_MAX;
}

void CollisionManager::AddCollider(Ezptr<AIMObject> _Obj)
{
	if (_Obj->IsAlive() == false)
	{
		return;
	}

	const std::list<Ezptr<AIMCollider>>* TmpColliderList = _Obj->GetColliderList();

	std::list<Ezptr<AIMCollider>>::const_iterator StartIter = TmpColliderList->begin();
	std::list<Ezptr<AIMCollider>>::const_iterator EndIter = TmpColliderList->end();

	if (TmpColliderList->empty() == true)
	{
		return;
	}
	else if (_Obj->IsEnable() == false)
	{
		for (; StartIter != EndIter; ++StartIter)
		{
			Ezptr<AIMCollider> Tmp = (*StartIter);
			Tmp->ClearState();
		}
	}

	// 오브젝트의 충돌체 리스트를 돈다
	StartIter = TmpColliderList->begin();

	for (; StartIter != EndIter; ++StartIter)
	{
		Ezptr<AIMCollider> Collider = (*StartIter);

		if (Collider->IsEnable() == false)
		{
			Collider->ClearState();
			continue;
		}
		else if (Collider->GetPickEnable() == true)
		{
			Collider->ClearState();
			continue;
		}

		// 충돌체의 MinMax를 가져온다
		Vec3 ColMin = Collider->GetSectionMin();
		Vec3 ColMax = Collider->GetSectionMax();

		// 섹션의 Min값을 빼서 공간보정.
		ColMin -= Section->Min;
		ColMax -= Section->Min;

		// 공간변환
		ColMin /= Section->CellSize;
		ColMax /= Section->CellSize;

		int StartX = -1, StartY = -1, StartZ = -1;
		int EndX = -1, EndY = -1, EndZ = -1;

		StartX = (int)ColMin.x;
		StartY = (int)ColMin.y;
		StartZ = (int)ColMin.z;

		EndX = (int)ColMax.x;
		EndY = (int)ColMax.y;
		EndZ = (int)ColMax.z;

		// 공간 밖에있는지 체크
		if (StartX >= Section->NumX)
		{
			continue;
		}
		else if (StartY >= Section->NumY)
		{
			continue;
		}
		else if (StartZ >= Section->NumZ)
		{
			continue;
		}

		// 0보다 작으면 0으로 만듦;
		StartX = StartX < 0 ? 0 : StartX;
		StartY = StartY < 0 ? 0 : StartY;
		StartZ = StartZ < 0 ? 0 : StartZ;

		if (EndX < 0)
		{
			continue;
		}
		else if (EndY <0)
		{
			continue;
		}
		else if (EndZ <0)
		{
			continue;
		}

		// 섹션의 최대값보다 크거나 같으면 섹션의 최대값으로
		EndX = EndX >= Section->NumX ? Section->NumX - 1 : EndX;
		EndY = EndY >= Section->NumY ? Section->NumY - 1 : EndY;
		EndZ = EndZ >= Section->NumZ ? Section->NumZ - 1 : EndZ;

		for (int z = 0; z <= EndZ; z++)
		{
			for (int y = 0; y <= EndY; y++)
			{
				for (int x = 0; x <= EndZ; x++)
				{
					// 섹션의 인덱스 구하고
					int Idx = z * (Section->NumX * Section->NumY) + y * Section->NumX + x;

					// 해당 섹션의 정보를 가져와서
					ColliderList* TmpSection = &Section->SectionList[Idx];

					// 사이즈가 작으면 늘리고
					if (TmpSection->Size == TmpSection->Capacity)
					{
						TmpSection->Capacity *= 2;

						AIMCollider** List = new AIMCollider*[TmpSection->Capacity];

						memcpy(List, TmpSection->List, sizeof(AIMCollider*) * TmpSection->Size);

						if (TmpSection->List != nullptr)
						{
							delete[] TmpSection->List;
							TmpSection->List = nullptr;
						}

						TmpSection->List = List;
					}

					// 해당 섹션의 리스트에 이 충돌체를 넣는다.
					TmpSection->List[TmpSection->Size] = Collider;
					++TmpSection->Size;
				}
			}
		}

	}
}

void CollisionManager::AddCollider(Ezptr<AIMCollider> _Collider)
{
 	ColInfo.ColliderList[_Collider->GetSerialNumber()] = _Collider;
	++ColInfo.Size;
}

Ezptr<AIMCollider> CollisionManager::FindCollider(unsigned int _SerialNumber)
{
	return ColInfo.ColliderList[_SerialNumber];
}

void CollisionManager::DeleteCollider(unsigned int _SerialNumber)
{
	ColInfo.ColliderList[_SerialNumber] = nullptr;
}

void CollisionManager::ComputeSection()
{
	for (unsigned int Col = 0; Col < ColInfo.Size + ValidSize; ++Col)
	{
		AIMCollider* TmpCollider = ColInfo.ColliderList[Col];

		if (false == TmpCollider)
			continue;

		if (false == TmpCollider->IsEnable() || false == TmpCollider->IsObjectEnable())
		{
			TmpCollider->ClearState();
			continue;
		}

		// 충돌체의 Min, Max 값을 얻어온다.
		Vec3 ColMin = TmpCollider->GetSectionMin();
		Vec3 ColMax = TmpCollider->GetSectionMax();

		// 전체 영역의 Min값을 제거해서 0, 0, 0으로 만들어준다.
		ColMin -= Section->Min;
		ColMax -= Section->Min;

		// 1, 1, 1 공간으로 변환한다.
		ColMin /= Section->CellSize;
		ColMax /= Section->CellSize;

		int	StartX = -1, StartY = -1, StartZ = -1;
		int	EndX = -1, EndY = -1, EndZ = -1;

		StartX = (int)ColMin.x;
		StartY = (int)ColMin.y;
		StartZ = (int)ColMin.z;

		EndX = (int)ColMax.x;
		EndY = (int)ColMax.y;
		EndZ = (int)ColMax.z;

		if (StartX >= Section->NumX || StartY >= Section->NumY ||
			StartZ >= Section->NumZ || EndX < 0 || EndY < 0 ||
			EndZ < 0)
		{
			TmpCollider->ClearState();
			continue;
		}

		StartX = StartX < 0 ? 0 : StartX;
		StartY = StartY < 0 ? 0 : StartY;
		StartZ = StartZ < 0 ? 0 : StartZ;

		EndX = EndX >= Section->NumX ? Section->NumX - 1 : EndX;
		EndY = EndY >= Section->NumY ? Section->NumY - 1 : EndY;
		EndZ = EndZ >= Section->NumZ ? Section->NumZ - 1 : EndZ;

		for (int z = StartZ; z <= EndZ; ++z)
		{
			for (int y = StartY; y <= EndY; ++y)
			{
				for (int x = StartX; x <= EndX; ++x)
				{
					int	idx = z * (Section->NumX * Section->NumY) +
						y * Section->NumX + x;

					ColliderList* TmpSection = &Section->SectionList[idx];

					if (TmpCollider->GetPickEnable())
					{
						if (Core::Inst()->GetEditMode())
							TmpSection = &PickSection->SectionList[idx];

						else
							continue;
					}

					if (TmpSection->Size == TmpSection->Capacity)
					{
						TmpSection->Capacity *= 2;

						AIMCollider** pList = new AIMCollider*[TmpSection->Capacity];

						memcpy(pList, TmpSection->List, sizeof(AIMCollider*) * TmpSection->Size);

						delete[] TmpSection->List;
						TmpSection->List = nullptr;

						TmpSection->List = pList;
					}

					TmpSection->List[TmpSection->Size] = TmpCollider;
					++TmpSection->Size;
				}
			}
		}
	}
}

bool CollisionManager::CollisionMouseUI(float _Time)
{
	return false;
}

bool CollisionManager::CollisionMouseWorld(float _Time)
{
	// 마우스 광선을 얻어온다.
	AIMColliderRay*	pMouseRay = InputManager::GetMouseRay();

	RayInfo	tRayInfo = pMouseRay->GetInfo();

	Vec3	vCollMin, vCollMax;
	Vec3	vPos1, vPos2;
	vPos1 = tRayInfo.Origin;
	vPos2 = tRayInfo.Origin + tRayInfo.Dir * 1500.f;

	vCollMin.x = vPos1.x < vPos2.x ? vPos1.x : vPos2.x;
	vCollMin.y = vPos1.y < vPos2.y ? vPos1.y : vPos2.y;
	vCollMin.z = vPos1.z < vPos2.z ? vPos1.z : vPos2.z;

	vCollMax.x = vPos1.x > vPos2.x ? vPos1.x : vPos2.x;
	vCollMax.y = vPos1.y > vPos2.y ? vPos1.y : vPos2.y;
	vCollMax.z = vPos1.z > vPos2.z ? vPos1.z : vPos2.z;

	vCollMin /= Section->CellSize;
	vCollMax /= Section->CellSize;

	int	iStartX = -1, iStartY = -1, iStartZ = -1;
	int	iEndX = -1, iEndY = -1, iEndZ = -1;

	iStartX = (int)vCollMin.x;
	iStartY = (int)vCollMin.y;
	iStartZ = (int)vCollMin.z;

	iEndX = (int)vCollMax.x;
	iEndY = (int)vCollMax.y;
	iEndZ = (int)vCollMax.z;

	iStartX = iStartX < 0 ? 0 : iStartX;
	iStartY = iStartY < 0 ? 0 : iStartY;
	iStartZ = iStartZ < 0 ? 0 : iStartZ;

	iEndX = iEndX >= Section->NumX ? Section->NumX - 1 : iEndX;
	iEndY = iEndY >= Section->NumY ? Section->NumY - 1 : iEndY;
	iEndZ = iEndZ >= Section->NumZ ? Section->NumZ - 1 : iEndZ;

	bool	bEditPick = false;

	if (Core::Inst()->GetEditMode())
	{
		for (int z = iStartZ; z <= iEndZ; ++z)
		{
			for (int y = iStartY; y <= iEndY; ++y)
			{
				for (int x = iStartX; x <= iEndX; ++x)
				{
					int	idx = z * (PickSection->NumX * PickSection->NumY) +
						y * PickSection->NumX + x;

					PColliderList	pSection = &PickSection->SectionList[idx];

					for (int i = 0; i < pSection->Size; ++i)
					{
						MouseCollisionList->Add(pSection->List[i]);
					}
				}
			}
		}

		// 추가된 충돌체들을 거리 순으로 오름차순 정렬한다.
		qsort(MouseCollisionList->List, MouseCollisionList->Size,
			sizeof(AIMCollider*), CollisionManager::SortZ);

		for (int i = 0; i < MouseCollisionList->Size; ++i)
		{
			AIMCollider*	pDest = MouseCollisionList->List[i];

			if (pMouseRay->Collision(pDest))
			{
				if (PrevMousePick && PrevMousePick != pDest)
				{
					pMouseRay->DeleteSerialNumber(PrevMousePick->GetSerialNumber());
					PrevMousePick->DeleteSerialNumber(pMouseRay->GetSerialNumber());

					DeletePrevCollider(pMouseRay->GetSerialNumber(),
						PrevMousePick->GetSerialNumber());
				}

				// 각 충돌체에 서로 충돌된 충돌체라고 등록해준다.
				// 현재 영역에서 충돌되었다고 등록을 해준다.
				// 이 리스트는 다음 영역에서 충돌체크시에 이전 영역에서
				// 충돌되었다면 빠져나가기 위함이다.
				// 왜냐하면 지금 서로 다른 영역에 걸쳐있을때 앞에 체크한
				// 영역에서 만약 충돌이 되었다면 이 충돌체는 계속
				// 충돌상태라고 표현이 되어버리므로 이전 영역에서
				// 충돌이 되었는지를 판단해야 한다.
				pMouseRay->AddCollisionList(pDest->GetSerialNumber());
				pDest->AddCollisionList(pMouseRay->GetSerialNumber());

				// 이전에 충돌되었는지를 판단한다.
				// 처음 충돌될 경우
				if (!CheckPrevCollider(pMouseRay->GetSerialNumber(),
					pDest->GetSerialNumber()))
				{
					// 충돌 매트릭스에 이전충돌목록으로
					// 등록해준다.
					AddPrevCollider(pMouseRay->GetSerialNumber(),
						pDest->GetSerialNumber());

					// 각 충돌체에 이전 충돌목록으로 등록한다.
					pMouseRay->AddSerialNumber(pDest->GetSerialNumber());
					pDest->AddSerialNumber(pMouseRay->GetSerialNumber());

					// 처음 충돌되었으므로 처음 충돌되었을때 호출할
					// 콜백을 처리한다.
					pMouseRay->Call(CCS_BEGIN, pDest, _Time);
					pDest->Call(CCS_BEGIN, pMouseRay, _Time);
				}

				// 이전 충돌목록에 있을 경우 계속 충돌상태로
				// 처리한다.
				else
				{
					// 콜백을 처리한다.
					pMouseRay->Call(CCS_STAY, pDest, _Time);
					pDest->Call(CCS_STAY, pMouseRay, _Time);
				}

				bEditPick = true;

				PrevMousePick = pDest;
				break;
			}

			// 충돌이 안된 상태일 경우
			else
			{
				// 이전 충돌목록에 서로 존재할 경우 충돌 되다가
				// 떨어진다는 것이다.
				if (CheckPrevCollider(pMouseRay->GetSerialNumber(),
					pDest->GetSerialNumber()))
				{
					// 이전 충돌목록에서 제거해준다.
					DeletePrevCollider(pMouseRay->GetSerialNumber(),
						pDest->GetSerialNumber());

					pMouseRay->DeleteSerialNumber(pDest->GetSerialNumber());
					pDest->DeleteSerialNumber(pMouseRay->GetSerialNumber());

					// 콜백을 처리한다.
					pMouseRay->Call(CCS_LEAVE, pDest, _Time);
					pDest->Call(CCS_LEAVE, pMouseRay, _Time);
				}
			}
		}

		MouseCollisionList->Clear();
	}

	if (!bEditPick)
	{
		if (PrevMousePick)
		{
			pMouseRay->DeleteSerialNumber(PrevMousePick->GetSerialNumber());
			PrevMousePick->DeleteSerialNumber(pMouseRay->GetSerialNumber());

			DeletePrevCollider(pMouseRay->GetSerialNumber(),
				PrevMousePick->GetSerialNumber());
		}

		PrevMousePick = nullptr;
	}

	for (int z = iStartZ; z <= iEndZ; ++z)
	{
		for (int y = iStartY; y <= iEndY; ++y)
		{
			for (int x = iStartX; x <= iEndX; ++x)
			{
				int	idx = z * (Section->NumX * Section->NumY) +
					y * Section->NumX + x;

				PColliderList	pSection = &Section->SectionList[idx];

				for (int i = 0; i < pSection->Size; ++i)
				{
					MouseCollisionList->Add(pSection->List[i]);
				}
			}
		}
	}

	// 추가된 충돌체들을 거리 순으로 오름차순 정렬한다.
	qsort(MouseCollisionList->List, MouseCollisionList->Size,
		sizeof(AIMCollider*), CollisionManager::SortZ);

	for (int i = 0; i < MouseCollisionList->Size; ++i)
	{
		AIMCollider*	pDest = MouseCollisionList->List[i];

		if (pMouseRay->Collision(pDest))
		{
			if (PrevMouseCollider && PrevMouseCollider != pDest)
			{
				pMouseRay->DeleteSerialNumber(PrevMouseCollider->GetSerialNumber());
				PrevMouseCollider->DeleteSerialNumber(pMouseRay->GetSerialNumber());

				DeletePrevCollider(pMouseRay->GetSerialNumber(),
					PrevMouseCollider->GetSerialNumber());
			}
			// 각 충돌체에 서로 충돌된 충돌체라고 등록해준다.
			// 현재 영역에서 충돌되었다고 등록을 해준다.
			// 이 리스트는 다음 영역에서 충돌체크시에 이전 영역에서
			// 충돌되었다면 빠져나가기 위함이다.
			// 왜냐하면 지금 서로 다른 영역에 걸쳐있을때 앞에 체크한
			// 영역에서 만약 충돌이 되었다면 이 충돌체는 계속
			// 충돌상태라고 표현이 되어버리므로 이전 영역에서
			// 충돌이 되었는지를 판단해야 한다.
			pMouseRay->AddCollisionList(pDest->GetSerialNumber());
			pDest->AddCollisionList(pMouseRay->GetSerialNumber());

			// 이전에 충돌되었는지를 판단한다.
			// 처음 충돌될 경우
			if (!CheckPrevCollider(pMouseRay->GetSerialNumber(),
				pDest->GetSerialNumber()))
			{
				// 충돌 매트릭스에 이전충돌목록으로
				// 등록해준다.
				AddPrevCollider(pMouseRay->GetSerialNumber(),
					pDest->GetSerialNumber());

				// 각 충돌체에 이전 충돌목록으로 등록한다.
				pMouseRay->AddSerialNumber(pDest->GetSerialNumber());
				pDest->AddSerialNumber(pMouseRay->GetSerialNumber());

				// 처음 충돌되었으므로 처음 충돌되었을때 호출할
				// 콜백을 처리한다.
				pMouseRay->Call(CCS_BEGIN, pDest, _Time);
				pDest->Call(CCS_BEGIN, pMouseRay, _Time);
			}

			// 이전 충돌목록에 있을 경우 계속 충돌상태로
			// 처리한다.
			else
			{
				// 콜백을 처리한다.
				pMouseRay->Call(CCS_STAY, pDest, _Time);
				pDest->Call(CCS_STAY, pMouseRay, _Time);
			}

			PrevMouseCollider = pDest;

			MouseCollisionList->Clear();
			return true;
		}

		// 충돌이 안된 상태일 경우
		else
		{
			// 이전 충돌목록에 서로 존재할 경우 충돌 되다가
			// 떨어진다는 것이다.
			if (CheckPrevCollider(pMouseRay->GetSerialNumber(),
				pDest->GetSerialNumber()))
			{
				// 이전 충돌목록에서 제거해준다.
				DeletePrevCollider(pMouseRay->GetSerialNumber(),
					pDest->GetSerialNumber());

				pMouseRay->DeleteSerialNumber(pDest->GetSerialNumber());
				pDest->DeleteSerialNumber(pMouseRay->GetSerialNumber());

				// 콜백을 처리한다.
				pMouseRay->Call(CCS_LEAVE, pDest, _Time);
				pDest->Call(CCS_LEAVE, pMouseRay, _Time);
			}
		}
	}

	if (PrevMouseCollider)
	{
		pMouseRay->DeleteSerialNumber(PrevMouseCollider->GetSerialNumber());
		PrevMouseCollider->DeleteSerialNumber(pMouseRay->GetSerialNumber());

		DeletePrevCollider(pMouseRay->GetSerialNumber(),
			PrevMouseCollider->GetSerialNumber());
		PrevMouseCollider = nullptr;
	}

	MouseCollisionList->Clear();

	return false;
}

bool CollisionManager::CollisionUI(float _Time)
{
	return false;
}

bool CollisionManager::CollisionWorld(float _Time)
{
	for (unsigned int z = 0; z < Section->NumZ; ++z)
	{
		for (unsigned int y = 0; y < Section->NumY; ++y)
		{
			for (unsigned int x = 0; x < Section->NumX; ++x)
			{
				unsigned int	idx = z * (Section->NumX * Section->NumY) +
					y * Section->NumX + x;

				PColliderList	pSection = &Section->SectionList[idx];

				if (pSection->Size <= 1)
					continue;

				for (unsigned int i = 0; i < pSection->Size - 1; ++i)
				{
					AIMCollider*	pSrc = pSection->List[i];
					for (unsigned int j = i + 1; j < pSection->Size; ++j)
					{
						AIMCollider*	pDest = pSection->List[j];

						// 현재 다른 섹션에서 충돌되었는지를 판단한다.
						if (pSrc->CheckCollisionList(pDest->GetSerialNumber()))
							continue;

						// 두 충돌체가 사용하는 Profile을 얻어온다.
						PCollisionProfile	pSrcProfile = pSrc->GetCollisionProfile();
						PCollisionProfile	pDestProfile = pDest->GetCollisionProfile();

						PCollisionChannel	pSrcChannel = &pSrcProfile->ChannelList[pDest->GetCollisionChannelIndex()];
						PCollisionChannel	pDestChannel = &pDestProfile->ChannelList[pSrc->GetCollisionChannelIndex()];

						if (pSrcChannel->State != CCS_IGNORE ||
							pDestChannel->State != CCS_IGNORE)
						{
							if (pSrc->Collision(pDest))
							{
								// 각 충돌체에 서로 충돌된 충돌체라고 등록해준다.
								// 현재 영역에서 충돌되었다고 등록을 해준다.
								// 이 리스트는 다음 영역에서 충돌체크시에 이전 영역에서
								// 충돌되었다면 빠져나가기 위함이다.
								// 왜냐하면 지금 서로 다른 영역에 걸쳐있을때 앞에 체크한
								// 영역에서 만약 충돌이 되었다면 이 충돌체는 계속
								// 충돌상태라고 표현이 되어버리므로 이전 영역에서
								// 충돌이 되었는지를 판단해야 한다.
								pSrc->AddCollisionList(pDest->GetSerialNumber());
								pDest->AddCollisionList(pSrc->GetSerialNumber());

								// 이전에 충돌되었는지를 판단한다.
								// 처음 충돌될 경우
								if (!CheckPrevCollider(pSrc->GetSerialNumber(),
									pDest->GetSerialNumber()))
								{
									// 충돌 매트릭스에 이전충돌목록으로
									// 등록해준다.
									AddPrevCollider(pSrc->GetSerialNumber(),
										pDest->GetSerialNumber());

									// 각 충돌체에 이전 충돌목록으로 등록한다.
									pSrc->AddSerialNumber(pDest->GetSerialNumber());
									pDest->AddSerialNumber(pSrc->GetSerialNumber());

									// 처음 충돌되었으므로 처음 충돌되었을때 호출할
									// 콜백을 처리한다.
									if (pSrcChannel->State != CCS_IGNORE)
										pSrc->Call(CCS_BEGIN, pDest, _Time);

									if (pDestChannel->State != CCS_IGNORE)
										pDest->Call(CCS_BEGIN, pSrc, _Time);
								}

								// 이전 충돌목록에 있을 경우 계속 충돌상태로
								// 처리한다.
								else
								{
									// 콜백을 처리한다.
									if (pSrcChannel->State != CCS_IGNORE)
										pSrc->Call(CCS_STAY, pDest, _Time);

									if (pDestChannel->State != CCS_IGNORE)
										pDest->Call(CCS_STAY, pSrc, _Time);
								}
							}

							// 충돌이 안된 상태일 경우
							else
							{
								// 이전 충돌목록에 서로 존재할 경우 충돌 되다가
								// 떨어진다는 것이다.
								if (CheckPrevCollider(pSrc->GetSerialNumber(),
									pDest->GetSerialNumber()))
								{
									// 이전 충돌목록에서 제거해준다.
									DeletePrevCollider(pSrc->GetSerialNumber(),
										pDest->GetSerialNumber());

									pSrc->DeleteSerialNumber(pDest->GetSerialNumber());
									pDest->DeleteSerialNumber(pSrc->GetSerialNumber());

									// 콜백을 처리한다.
									if (pSrcChannel->State != CCS_IGNORE)
										pSrc->Call(CCS_LEAVE, pDest, _Time);

									if (pDestChannel->State != CCS_IGNORE)
										pDest->Call(CCS_LEAVE, pSrc, _Time);
								}
							}
						}

						else
						{
							if (CheckPrevCollider(pSrc->GetSerialNumber(),
								pDest->GetSerialNumber()))
							{
								// 이전 충돌목록에서 제거해준다.
								DeletePrevCollider(pSrc->GetSerialNumber(),
									pDest->GetSerialNumber());

								pSrc->DeleteSerialNumber(pDest->GetSerialNumber());
								pDest->DeleteSerialNumber(pSrc->GetSerialNumber());

								// 콜백을 처리한다.
								if (pSrcChannel->State != CCS_IGNORE)
									pSrc->Call(CCS_LEAVE, pDest, _Time);

								if (pDestChannel->State != CCS_IGNORE)
									pDest->Call(CCS_LEAVE, pSrc, _Time);
							}
						}
					}
				}

				pSection->Size = 0;
			}
		}
	}

	return true;
}

bool CollisionManager::Collision(Ezptr<AIMCollider> _Src, Ezptr<AIMCollider> _Dest)
{
	return false;
}

int CollisionManager::SortZ(const void * _Src, const void * _Dest)
{
	AIMCollider* SrcCol = *((AIMCollider**)_Src);
	AIMCollider* DestCol = *((AIMCollider**)_Dest);

	Vec3 SrcMin = SrcCol->GetSectionMin();
	Vec3 DestMin = DestCol->GetSectionMin();
	Vec3 SrcMax = SrcCol->GetSectionMax();
	Vec3 DestMax = DestCol->GetSectionMax();

	Vec3 SrcCenter = (SrcMin + SrcMax) / 2.0f;
	Vec3 DestCenter = (DestMin + DestMax) / 2.0f;

	Vec3 CamPos = SceneManager::GetMainCameraPos();

	float SrcDist = SrcCenter.Distance(CamPos);
	float DestDist = DestCenter.Distance(CamPos);

	if (SrcDist > DestDist)
	{
		return 1;
	}
	else if (SrcDist < DestDist)
	{
		return -1;
	}

	return 0;
}
