#include "CollisionManager.h"
#include "AIMObject.h"

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


bool CollisionManager::Init()
{
	ProfileList = new CollisionProfile[ProfileCapacity];
	ChannelList = new CollisionChannel[MAX_COL_CHANNEL];
	ValidNumber = new unsigned int[ValidCapacity];
	memset(ValidNumber, 0, sizeof(unsigned int) * ValidCapacity);

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
}

void CollisionManager::Collision(float _Time)
{
	ComputeSection();

	for (unsigned int z = 0; z < Section->NumZ; z++)
	{
		for (unsigned int y = 0; y < Section->NumY; y++)
		{
			for (unsigned int x = 0; x < Section->NumX; x++)
			{
				unsigned int Idx = z * (Section->NumX * Section->NumY) + y * Section->NumX + x;

				ColliderList* TmpSection = &Section->SectionList[Idx];

				if (TmpSection->Size <= 1)
				{
					continue;
				}

				for (unsigned int AA = 0; AA < TmpSection->Size - 1; AA++)
				{
					Ezptr<AIMCollider> Src = TmpSection->List[AA];

					for (unsigned int BB = AA + 1; BB < TmpSection->Size; BB++)
					{
						Ezptr<AIMCollider> Dest = TmpSection->List[BB];

						if (Src->CheckCollisionList(Dest->GetSerialNumber()) == true)
						{
							continue;
						}

						CollisionProfile* SrcProfile = Src->GetCollisionProfile();
						CollisionProfile* DestProfile = Dest->GetCollisionProfile();

						CollisionChannel* SrcChannel = &SrcProfile->ChannelList[Dest->GetCollisionChannelIndex()];
						CollisionChannel* DestChannel = &DestProfile->ChannelList[Src->GetCollisionChannelIndex()];

						if (SrcChannel->State != CCS_IGNORE || DestChannel->State != CCS_IGNORE)
						{
							if (Src->Collision(Dest) == true)
							{
								// ���� �������� �浹 �Ǿ�����
								// �浹�� �ߺ��� ���ϱ� ���ؼ�
								// ���� �����ӿ��� �浹�Ǿ��ٰ� ǥ�ø� ���ش�

								Src->AddCollisionList(Dest->GetSerialNumber());
								Dest->AddCollisionList(Src->GetSerialNumber());

								// ���� �浹 �Ǵ�
								if (CheckPrevCollider(Src->GetSerialNumber(), Dest->GetSerialNumber()) == false)
								{
									// �����浹�̹Ƿ� �����
									AddPrevCollider(Src->GetSerialNumber(), Dest->GetSerialNumber());

									// �� �浹ü���� �����
									Src->AddSerialNumber(Dest->GetSerialNumber());
									Dest->AddSerialNumber(Src->GetSerialNumber());

									// �����ߵ� �ݹ�
									if (SrcChannel->State != CCS_IGNORE)
									{
										Src->Call(CCS_BEGIN, Dest, _Time);
									}

									if (DestChannel->State != CCS_IGNORE)
									{
										Dest->Call(CCS_BEGIN, Src, _Time);
									}
								}
								else
								{
									if (SrcChannel->State != CCS_IGNORE)
									{
										Src->Call(CCS_STAY, Dest, _Time);
									}

									if (DestChannel->State != CCS_IGNORE)
									{
										Dest->Call(CCS_STAY, Src, _Time);
									}
								}
							}
							// �浹 ����
							else
							{
								if (CheckPrevCollider(Src->GetSerialNumber(), Dest->GetSerialNumber()) == true)
								{
									DeletePrevCollider(Src->GetSerialNumber(), Dest->GetSerialNumber());

									Src->DeleteSerialNumber(Dest->GetSerialNumber());
									Dest->DeleteSerialNumber(Src->GetSerialNumber());

									if (SrcChannel->State != CCS_IGNORE)
									{
										Src->Call(CCS_LEAVE, Dest, _Time);
									}

									if (DestChannel->State != CCS_IGNORE)
									{
										Dest->Call(CCS_LEAVE, Src, _Time);
									}
								}
							}
						}
						else
						{
							if (CheckPrevCollider(Src->GetSerialNumber(), Dest->GetSerialNumber()) == true)
							{
								DeletePrevCollider(Src->GetSerialNumber(), Dest->GetSerialNumber());

								Src->DeleteSerialNumber(Dest->GetSerialNumber());
								Dest->DeleteSerialNumber(Src->GetSerialNumber());

								if (SrcChannel->State != CCS_IGNORE)
								{
									Src->Call(CCS_LEAVE, Dest, _Time);
								}

								if (DestChannel->State != CCS_IGNORE)
								{
									Dest->Call(CCS_LEAVE, Src, _Time);
								}
							}
						}
					}
				}

				TmpSection->Size = 0;
			}
		}
	}
}

void CollisionManager::Render(float _Time)
{
	for (unsigned int i = 0; i < ColInfo.Size + ValidSize;)
	{
		Ezptr<AIMCollider> Collider = ColInfo.ColliderList[i];

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
	// 32��Ʈ�� ����ϹǷ� �����۾�

	// 100���̶��
	// Index = 3, Bit = 4�� ��
	unsigned int SrcIndex = _Src / 32;
	unsigned int DestIndex = _Dest / 32;

	// ��Ʈ������ ���ؼ� ��Ʈ�� �Ű���
	unsigned int SrcBit = 31 - (_Src % 32);
	unsigned int DestBit = 31 - (_Dest % 32);

	unsigned int SrcValue = 1 << SrcBit;
	unsigned int DestValue = 1 << DestBit;

	// Src�� Dest�� ������ ��ġ�� ��Ʈ�� 1�� �������
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

	// Src�� Dest ������ ��ġ�� ��Ʈ�� 0���� �������
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
				// ä�� ����Ʈ�� ���� ä�ο������� ��ĭ�� ����
				ChannelList[BB] = ChannelList[BB + 1];
			}

			for (unsigned int BB = 0; BB < ProfileSize; BB++)
			{
				--ProfileList[BB].ChannelSize;
				for (unsigned int CC = AA; CC < ChannelSize; CC++)
				{
					// ��� ä���� �����ʿ��� ä�� ����Ʈ�� ��ĭ�� ����ش�
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

	// ������Ʈ�� �浹ü ����Ʈ�� ����
	StartIter = TmpColliderList->begin();

	for (; StartIter != EndIter; ++StartIter)
	{
		Ezptr<AIMCollider> Collider = (*StartIter);

		if (Collider->IsEnable() == false)
		{
			Collider->ClearState();
			continue;
		}

		// �浹ü�� MinMax�� �����´�
		Vec3 ColMin = Collider->GetSectionMin();
		Vec3 ColMax = Collider->GetSectionMax();

		// ������ Min���� ���� ��������.
		ColMin -= Section->Min;
		ColMax -= Section->Min;

		// ������ȯ
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

		// ���� �ۿ��ִ��� üũ
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

		// 0���� ������ 0���� ����;
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

		// ������ �ִ밪���� ũ�ų� ������ ������ �ִ밪����
		EndX = EndX >= Section->NumX ? Section->NumX - 1 : EndX;
		EndY = EndY >= Section->NumY ? Section->NumY - 1 : EndY;
		EndZ = EndZ >= Section->NumZ ? Section->NumZ - 1 : EndZ;

		for (int z = 0; z <= EndZ; z++)
		{
			for (int y = 0; y <= EndY; y++)
			{
				for (int x = 0; x <= EndZ; x++)
				{
					// ������ �ε��� ���ϰ�
					int Idx = z * (Section->NumX * Section->NumY) + y * Section->NumX + x;

					// �ش� ������ ������ �����ͼ�
					ColliderList* TmpSection = &Section->SectionList[Idx];

					// ����� ������ �ø���
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

					// �ش� ������ ����Ʈ�� �� �浹ü�� �ִ´�.
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
	for (unsigned int Coll = 0; Coll < ColInfo.Size + ValidSize; Coll++)
	{
		Ezptr<AIMCollider> Collider = ColInfo.ColliderList[Coll];

		if (Collider == nullptr)
		{
			continue;
		}

		if (Collider->IsEnable() == false || Collider->IsObjectEnable() == false)
		{
			Collider->ClearState();
			continue;
		}

		// �浹ü�� MinMax�� �����´�
		Vec3 ColMin = Collider->GetSectionMin();
		Vec3 ColMax = Collider->GetSectionMax();

		// ������ Min���� ���� ��������.
		ColMin -= Section->Min;
		ColMax -= Section->Min;

		// ������ȯ
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

		// ���� �ۿ��ִ��� üũ
		if (StartX >= Section->NumX)
		{
			Collider->ClearState();
			continue;
		}
		else if (StartY >= Section->NumY)
		{
			Collider->ClearState();
			continue;
		}
		else if (StartZ >= Section->NumZ)
		{
			Collider->ClearState();
			continue;
		}
		else if (EndX< 0)
		{
			Collider->ClearState();
			continue;
		}
		else if (EndY <0)
		{
			Collider->ClearState();
			continue;
		}
		else if (EndZ < 0)
		{
			Collider->ClearState();
			continue;
		}

		// 0���� ������ 0���� ����;
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

		// ������ �ִ밪���� ũ�ų� ������ ������ �ִ밪����
		EndX = EndX >= Section->NumX ? Section->NumX - 1 : EndX;
		EndY = EndY >= Section->NumY ? Section->NumY - 1 : EndY;
		EndZ = EndZ >= Section->NumZ ? Section->NumZ - 1 : EndZ;

		for (int z = 0; z <= EndZ; z++)
		{
			for (int y = 0; y <= EndY; y++)
			{
				for (int x = 0; x <= EndZ; x++)
				{
					// ������ �ε��� ���ϰ�
					int Idx = z * (Section->NumX * Section->NumY) + y * Section->NumX + x;

					// �ش� ������ ������ �����ͼ�
					ColliderList* TmpSection = &Section->SectionList[Idx];

					// ����� ������ �ø���
					if (TmpSection->Size == TmpSection->Capacity)
					{
						TmpSection->Capacity *= 2;

						AIMCollider** TmpList = new AIMCollider*[TmpSection->Capacity];

						memcpy(TmpList, TmpSection->List, sizeof(AIMCollider*) * TmpSection->Size);

						if (TmpSection->List != nullptr)
						{
							delete[] TmpSection->List;
							TmpSection->List = nullptr;
						}

						TmpSection->List = TmpList;
					}

					// �ش� ������ ����Ʈ�� �� �浹ü�� �ִ´�.
					TmpSection->List[TmpSection->Size] = Collider;
					++TmpSection->Size;
				}
			}
		}

	}
}
