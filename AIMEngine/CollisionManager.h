#pragma once

#include "AIMCollider.h"

#define MAX_COL_CHANNEL 32

// 관리자에서 들고있을 충돌체들의 정보
typedef struct Engine_DLL _tagColliderInfo
{
	// 다른 충돌체들의 포인터
	AIMCollider** ColliderList;
	// 충돌 체크용 인접행렬
	unsigned int** AdjMatrix;
	
	// 들고있는 실제 크기
	unsigned int Size;
	// 최대 수용 가능한 크기
	unsigned int Capacity;

	void Resize()
	{
		Capacity *= 2;

		AIMCollider** List = new AIMCollider*[Capacity];

		memcpy(List, ColliderList, sizeof(AIMCollider*) * Size);

		if (ColliderList != nullptr)
		{
			delete[] ColliderList;
			ColliderList = nullptr;
		}

		ColliderList = List;

		unsigned int** Matrix = new unsigned int*[Capacity];

		unsigned int Width = Capacity / 32;

		for (unsigned int i = 0; i < Capacity; i++)
		{
			Matrix[i] = new unsigned int[Width];
			memset(Matrix[i], 0, sizeof(unsigned int) * Width);
			memcpy(Matrix[i], AdjMatrix[i], sizeof(unsigned int) * (Width / 2));

			if (AdjMatrix[i] != nullptr)
			{
				delete[] AdjMatrix[i];
				AdjMatrix[i] = nullptr;
			}
		}

		if (AdjMatrix != nullptr)
		{
			delete[] AdjMatrix;
			AdjMatrix = nullptr;
		}

		AdjMatrix = Matrix;
	}

	_tagColliderInfo()
	{
		Size = 0;
		Capacity = 4096;
		ColliderList = new AIMCollider*[Capacity];
		memset(ColliderList, 0, sizeof(AIMCollider*) * Capacity);
		AdjMatrix = new unsigned int*[Capacity];

		unsigned int Width = Capacity / 32;

		for (unsigned int i = 0; i < Capacity; i++)
		{
			AdjMatrix[i] = new unsigned int[Width];
			memset(AdjMatrix[i], 0, sizeof(unsigned int) * Width);
		}
	}

	~_tagColliderInfo()
	{
		if (ColliderList != nullptr)
		{
			delete[] ColliderList;
			ColliderList = nullptr;
		}

		for (unsigned int i = 0; i < Capacity; i++)
		{
			if (AdjMatrix[i] != nullptr)
			{
				delete[] AdjMatrix[i];
				AdjMatrix[i] = nullptr;
			}
		}

		if (AdjMatrix != nullptr)
		{
			delete AdjMatrix;
			AdjMatrix = nullptr;
		}
	}
}ColliderInfo, *PColliderInfo;


typedef struct Engine_DLL _tagColliderList
{
	AIMCollider** List;
	int Size;
	int Capacity;

	_tagColliderList()
	{
		Size = 0;
		Capacity = 10;
		List = new AIMCollider*[Capacity];
		memset(List, 0, sizeof(AIMCollider*) * Capacity);
	}

	~_tagColliderList()
	{
		if (List != nullptr)
		{
			delete[] List;
			List = nullptr;
		}
	}
}ColliderList, *PColliderList;

// 충돌체의 레이어
typedef struct Engine_DLL _tagCollisionChannel
{
	std::string Name;
	CollisionChannelState State;

	_tagCollisionChannel()
	{
		Name = "";
		State = CCS_IGNORE;
	}
}CollisionChannel, *PCollisionChannel;

// 충돌체간의 상호작용을 관리할 구조체
typedef struct Engine_DLL _tagCollisionProfile
{
	std::string Name;
	CollisionChannel* ChannelList;
	unsigned int ChannelSize;

	CollisionChannel* FindChannel(const std::string& _Name)
	{
		for (unsigned int i = 0; i < ChannelSize; i++)
		{
			if (ChannelList[i].Name == _Name)
			{
				return &ChannelList[i];
			}
		}

		return nullptr;
	}

	_tagCollisionProfile()
	{
		Name = "";
		ChannelSize = 0;
		ChannelList = new CollisionChannel[MAX_COL_CHANNEL];
	}

	~_tagCollisionProfile()
	{
		if (ChannelList != nullptr)
		{
			delete[] ChannelList;
			ChannelList = nullptr;
		}
	}
}CollisionProfile, *PCollisionProfile;

// 실제 충돌 연산에서 사용할 구획을 담당하는 구조체
typedef struct Engine_DLL _tagCollisionSection
{
	ColliderList* SectionList;
	int NumX;
	int NumY;
	int NumZ;
	Vec3 Min;
	Vec3 Max;
	Vec3 CellSize;
	Vec3 Length;

	_tagCollisionSection() : NumX(1), NumY(1), NumZ(1), SectionList(nullptr)
	{

	}

	~_tagCollisionSection()
	{
		if (SectionList != nullptr)
		{
			delete[] SectionList;
			SectionList = nullptr;
		}
	}
}CollisionSection, *PCollisionSection;

class Engine_DLL CollisionManager
{
public:
	static bool Init();
	static void Release();
	static void Collision(float _Time);
	static void Render(float _Time);

private:
	static ColliderInfo ColInfo;
	static unsigned int SerialNumber;

	static unsigned int* ValidNumber;
	static unsigned int ValidSize;
	static unsigned int ValidCapacity;

private:
	static CollisionSection* Section;
	static Vec3 Pos;
	static Vec3 Pivot;

private:
	static CollisionProfile* ProfileList;
	static unsigned int ProfileSize;
	static unsigned int ProfileCapacity;

	static CollisionChannel* ChannelList;
	static unsigned int ChannelSize;

public:
	// 충돌체별 고유번호 부여
	static unsigned int GetSerialNumber();
	// 충돌체가 제거될때 고유번호 회수
	static void AddValidSerialNumber(unsigned int _Number);

	// 충돌인접행렬 원소 추가함수
	static void AddPrevCollider(unsigned int _Src, unsigned int _Dest);
	// 충돌인접행렬 원소 제거함수
	static void DeletePrevCollider(unsigned int _Src, unsigned int _Dest);
	// 충돌인접행렬 체크함수
	static bool CheckPrevCollider(unsigned int _Src, unsigned int _Dest);

public:
	static void CreateSection(int _NumX, int _NumY, int _NumZ, const Vec3& _CellSize, const Vec3& _Pos = Vec3::Zero, const Vec3& _Pivot = Vec3::Zero);
	static bool AddChannel(const std::string& _Name, CollisionChannelState _State = CCS_IGNORE);
	static bool DeleteChannel(const std::string& _Name);
	static bool CreateProfile(const std::string& _Name);
	static bool SetProfileChannelState(const std::string& _Name, const std::string& _Channel, CollisionChannelState _State);
	static CollisionProfile* FindProfile(const std::string& _Name);
	static unsigned int GetChannelIndex(const std::string& _Name);
	static void AddCollider(Ezptr<AIMObject> _Obj);
	static void AddCollider(Ezptr<AIMCollider> _Collider);

	static Ezptr<AIMCollider> FindCollider(unsigned int _SerialNumber);
	static void DeleteCollider(unsigned int _SerialNumber);
	static void ComputeSection();

private:
	CollisionManager() {}
	virtual ~CollisionManager() = 0 {}
};

