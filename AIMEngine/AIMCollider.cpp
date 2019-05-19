#include "AIMCollider.h"
#include "CollisionManager.h"

#ifdef _DEBUG
#include "AIMShader.h"
#include "ShaderManager.h"
#include "AIMMesh.h"
#include "RenderState.h"
#include "RenderManager.h"
#include "AIMDevice.h"
#endif


AIMCollider::AIMCollider()
{
	CT = CT_COLLIDER;

#ifdef _DEBUG
	Color = Vec4::Green;

	Shader = ShaderManager::FindShader("ColliderShader");
	Layout = ShaderManager::FindInputLayout("StandardPosLayout");
	Mesh = nullptr;
	WireFrame = RenderManager::FindRenderState("WireFrame");
#endif

	Profile = CollisionManager::FindProfile("WorldStatic");
}

AIMCollider::AIMCollider(const AIMCollider & _Other) : AIMComponent(_Other)
{
#ifdef _DEBUG
	Mesh = _Other.Mesh;
	Shader = _Other.Shader;
	Layout = _Other.Layout;
	WireFrame = _Other.WireFrame;
	Color = _Other.Color;
	CBuffer = _Other.CBuffer;
#endif

	ColType = _Other.ColType;
	SerialNumber = _Other.SerialNumber;
	_Other.SectionMin;
	_Other.SectionMax;
	Profile = _Other.Profile;
	ChannelIndex = _Other.ChannelIndex;
	PrevNumber = _Other.PrevNumber;
	PrevSize = _Other.PrevSize;
	PrevCapacity = _Other.PrevCapacity;
	CollisionList = _Other.CollisionList;
	CollisionSize = _Other.CollisionSize;
	CollisionCapacity = _Other.CollisionCapacity;

	memcpy(Callback, _Other.Callback, sizeof(std::function<void(Ezptr<AIMCollider>, Ezptr<AIMCollider>, float)>) * CCS_END);
	memcpy(bCallback, _Other.bCallback, sizeof(bool) * CCS_END);
	
}


AIMCollider::~AIMCollider()
{
	ClearState();

	if (PrevNumber != nullptr)
	{
		delete[] PrevNumber;
		PrevNumber = nullptr;
	}

	if (SerialNumber != UINT_MAX)
	{
		CollisionManager::AddValidSerialNumber(SerialNumber);
	}

	if (CollisionList != nullptr)
	{
		delete[] CollisionList;
		CollisionList = nullptr;
	}
}

_tagCollisionProfile * AIMCollider::GetCollisionProfile() const
{
	return Profile;
}

unsigned int AIMCollider::GetCollisionChannelIndex() const
{
	return ChannelIndex;
}

void AIMCollider::Start()
{
	SerialNumber = CollisionManager::GetSerialNumber();

	CollisionManager::AddCollider(this);

	PrevSize = 0;
	PrevCapacity = 10;
	PrevNumber = new unsigned int[PrevCapacity];

	CollisionSize = 0;
	CollisionCapacity = 10;
	CollisionList = new unsigned int[CollisionCapacity];
}

bool AIMCollider::Init()
{
	return true;
}

int AIMCollider::Input(float _Time)
{
	return 0;
}

int AIMCollider::Update(float _Time)
{
	return 0;
}

int AIMCollider::LateUpdate(float _Time)
{
	return 0;
}

int AIMCollider::Collision(float _Time)
{
	return 0;
}

int AIMCollider::PrevRender(float _Time)
{
	return 0;
}

int AIMCollider::Render(float _Time)
{
#ifdef _DEBUG
	if (PrevSize > 0)
	{
		Color = Vec4::Red;
	}
	else
	{
		Color = Vec4::Green;
	}

	WireFrame->SetState();

	ShaderManager::UpdateConstBuffer("Transform", &CBuffer);
	ShaderManager::UpdateConstBuffer("Collider", &Color);

	Shader->SetShader();
	GetAIMContext->IASetInputLayout(Layout);

	Mesh->Render();

	WireFrame->ResetState();
#endif

	CollisionSize = 0;

	return 0;
}

AIMCollider * AIMCollider::Clone() const
{
	return nullptr;
}

void AIMCollider::ClearState()
{
	if (SerialNumber == UINT_MAX)
	{
		return;
	}

	for (unsigned int i = 0; i < PrevSize; i++)
	{
		CollisionManager::DeletePrevCollider(SerialNumber, PrevNumber[i]);

		Ezptr<AIMCollider> Dest = CollisionManager::FindCollider(PrevNumber[i]);

		if (Dest != nullptr)
		{
			Dest->DeleteSerialNumber(SerialNumber);
		}

		PrevNumber[i] = UINT_MAX;
	}

	PrevSize = 0;
}

void AIMCollider::AddSerialNumber(unsigned int _SerailNumber)
{
	if (PrevSize == PrevCapacity)
	{
		PrevCapacity *= 2;

		unsigned int* List = new unsigned int[PrevCapacity];
		memcpy(List, PrevNumber, sizeof(unsigned int) * PrevSize);

		if (PrevNumber != nullptr)
		{
			delete[] PrevNumber;
			PrevNumber = nullptr;
		}

		PrevNumber = List;
	}

	PrevNumber[PrevSize] = _SerailNumber;
	++PrevSize;
}

void AIMCollider::DeleteSerialNumber(unsigned int _SerialNumber)
{
	for (unsigned int AA = 0; AA < PrevSize; AA++)
	{
		if (PrevNumber[AA] == _SerialNumber)
		{
			--PrevSize;
			for (unsigned int BB = 0; BB < PrevSize; BB++)
			{
				PrevNumber[BB] = PrevNumber[BB + 1];
			}

			PrevNumber[PrevSize] = UINT_MAX;
			break;
		}
	}
}

void AIMCollider::AddCollisionList(unsigned int _SerialNumber)
{
	if (CollisionSize == CollisionCapacity)
	{
		CollisionCapacity *= 2;

		unsigned int* List = new unsigned int[CollisionCapacity];
		memcpy(List, CollisionList, sizeof(unsigned int) * CollisionSize);

		if (CollisionList != nullptr)
		{
			delete[] CollisionList;
			CollisionList = nullptr;
		}

		CollisionList = List;
	}

	CollisionList[CollisionSize] = _SerialNumber;
	++CollisionSize;
}

void AIMCollider::DeleteCollisionList(unsigned int _SerialNumber)
{
	for (unsigned int AA = 0; AA < CollisionSize; AA++)
	{
		if (CollisionList[AA] == _SerialNumber)
		{
			--CollisionSize;

			for (unsigned int BB = 0; BB < CollisionSize; BB++)
			{
				CollisionList[BB] = CollisionList[BB + 1];
			}

			CollisionList[CollisionSize] = UINT_MAX;
			break;
		}
	}
}

void AIMCollider::ClearCollisionList()
{
	CollisionSize = 0;
}

bool AIMCollider::CheckCollisionList(unsigned int _SerialNumber)
{
	for (unsigned int i = 0; i < CollisionSize; i++)
	{
		if (CollisionList[i] == _SerialNumber)
		{
			return true;
		}
	}

	return false;
}

void AIMCollider::Call(CollisionCallbackState _State, Ezptr<AIMCollider> _Dest, float _Time)
{
	if (bCallback[_State] == true)
	{
		Callback[_State](this, _Dest, _Time);
	}
}

void AIMCollider::SetChannel(const std::string & _Name)
{
	ChannelIndex = CollisionManager::GetChannelIndex(_Name);
}

void AIMCollider::SetProfile(const std::string & _Name)
{
	Profile = CollisionManager::FindProfile(_Name);
}

bool AIMCollider::CollisionSp2Sp(const SphereInfo & _Src, const SphereInfo & _Dest)
{
	float Dist = _Src.Center.Distance(_Dest.Center);

	return Dist <= _Src.Radius + _Dest.Radius;
}

bool AIMCollider::CollisionOBB2OBB(const OBBInfo & _Src, const OBBInfo & _Dest)
{
	Vec3 Dir = _Dest.Center - _Src.Center;

	float AxisDot[3][3] = {};
	float AbsAxisDot[3][3] = {};
	float DirDot[3] = {};
	float r, r1, r2;
	bool Axis = false;

	for (int i = 0; i < 3; i++)
	{
		AxisDot[AXIS_X][i] = _Src.Axis[AXIS_X].Dot(_Dest.Axis[i]);
		AbsAxisDot[AXIS_X][i] = abs(AxisDot[AXIS_X][i]);

		if (AbsAxisDot[AXIS_X][i] > 0.99999f)
		{
			Axis = true;
		}
	}

	DirDot[0] = Dir.Dot(_Src.Axis[AXIS_X]);
	r = abs(DirDot[0]);
	r1 = _Src.HalfLength.x;
	r2 = _Dest.HalfLength.x * AbsAxisDot[AXIS_X][AXIS_X] + _Dest.HalfLength.y * AbsAxisDot[AXIS_X][AXIS_Y] + _Dest.HalfLength.z * AbsAxisDot[AXIS_X][AXIS_Z];

	if (r > r1 + r2)
	{
		return false;
	}

	for (int i = 0; i < 3; i++)
	{
		AxisDot[AXIS_Y][i] = _Src.Axis[AXIS_Y].Dot(_Dest.Axis[i]);
		AbsAxisDot[AXIS_Y][i] = abs(AxisDot[AXIS_Y][i]);

		if (AbsAxisDot[AXIS_Y][i] > 0.99999f)
		{
			Axis = true;
		}
	}

	DirDot[1] = Dir.Dot(_Src.Axis[AXIS_Y]);
	r = abs(DirDot[1]);
	r1 = _Src.HalfLength.y;
	r2 = _Dest.HalfLength.x * AbsAxisDot[AXIS_Y][AXIS_X] + _Dest.HalfLength.y * AbsAxisDot[AXIS_Y][AXIS_Y] + _Dest.HalfLength.z * AbsAxisDot[AXIS_Y][AXIS_Z];

	if (r > r1 + r2)
	{
		return false;
	}

	for (int i = 0; i < 3; i++)
	{
		AxisDot[AXIS_Z][i] = _Src.Axis[AXIS_Z].Dot(_Dest.Axis[i]);
		AbsAxisDot[AXIS_Z][i] = abs(AxisDot[AXIS_Z][i]);

		if (AbsAxisDot[AXIS_Z][i] > 0.99999f)
		{
			Axis = true;
		}
	}

	DirDot[2] = Dir.Dot(_Src.Axis[AXIS_Z]);
	r = abs(DirDot[2]);
	r1 = _Src.HalfLength.z;
	r2 = _Dest.HalfLength.x * AbsAxisDot[AXIS_Z][AXIS_X] + _Dest.HalfLength.y * AbsAxisDot[AXIS_Z][AXIS_Y] + _Dest.HalfLength.z * AbsAxisDot[AXIS_Z][AXIS_Z];

	if (r > r1 + r2)
	{
		return false;
	}

	r = abs(Dir.Dot(_Dest.Axis[AXIS_X]));
	r1 = _Src.HalfLength.x * AbsAxisDot[AXIS_X][AXIS_X] + _Src.HalfLength.y * AbsAxisDot[AXIS_Y][AXIS_X] + _Src.HalfLength.z * AbsAxisDot[AXIS_Z][AXIS_X];
	r2 = _Dest.HalfLength.x;

	if (r > r1 + r2)
	{
		return false;
	}

	r = abs(Dir.Dot(_Dest.Axis[AXIS_Y]));
	r1 = _Src.HalfLength.x * AbsAxisDot[AXIS_X][AXIS_Y] + _Src.HalfLength.y * AbsAxisDot[AXIS_Y][AXIS_Y] + _Src.HalfLength.z * AbsAxisDot[AXIS_Z][AXIS_Y];
	r2 = _Dest.HalfLength.y;

	if (r > r1 + r2)
	{
		return false;
	}

	r = abs(Dir.Dot(_Dest.Axis[AXIS_Z]));
	r1 = _Src.HalfLength.x * AbsAxisDot[AXIS_X][AXIS_Z] + _Src.HalfLength.y * AbsAxisDot[AXIS_Y][AXIS_Z] + _Src.HalfLength.z * AbsAxisDot[AXIS_Z][AXIS_Z];
	r2 = _Dest.HalfLength.z;

	if (r > r1 + r2)
	{
		return false;
	}

	if (Axis == true)
	{
		return true;
	}

	r = abs(DirDot[AXIS_Z] * AxisDot[AXIS_Y][AXIS_Y] - DirDot[AXIS_Y] * AxisDot[AXIS_Z][AXIS_Y]);
	r1 = _Src.HalfLength.y * AbsAxisDot[AXIS_Z][AXIS_Y] + _Src.HalfLength.z * AbsAxisDot[AXIS_Y][AXIS_Y];
	r2 = _Dest.HalfLength.x * AbsAxisDot[AXIS_X][AXIS_Z] + _Dest.HalfLength.z * AbsAxisDot[AXIS_X][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(DirDot[AXIS_Z] * AxisDot[AXIS_Y][AXIS_Z] - DirDot[AXIS_Y] * AxisDot[AXIS_Z][AXIS_Z]);
	r1 = _Src.HalfLength.y * AbsAxisDot[AXIS_Z][AXIS_Z] + _Src.HalfLength.z * AbsAxisDot[AXIS_Y][AXIS_Z];
	r2 = _Dest.HalfLength.x * AbsAxisDot[AXIS_X][AXIS_Y] + _Dest.HalfLength.y * AbsAxisDot[AXIS_X][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(DirDot[AXIS_X] * AxisDot[AXIS_Z][AXIS_X] - DirDot[AXIS_Y] * AxisDot[AXIS_Z][AXIS_Z]);
	r1 = _Src.HalfLength.x * AbsAxisDot[AXIS_Z][AXIS_X] + _Src.HalfLength.z * AbsAxisDot[AXIS_X][AXIS_X];
	r2 = _Dest.HalfLength.y * AbsAxisDot[AXIS_Y][AXIS_Z] + _Dest.HalfLength.z * AbsAxisDot[AXIS_Y][AXIS_Y];

	if (r > r1 + r2)
		return false;

	r = abs(DirDot[AXIS_X] * AxisDot[AXIS_Z][AXIS_Y] - DirDot[AXIS_Z] * AxisDot[AXIS_X][AXIS_Y]);
	r1 = _Src.HalfLength.x * AbsAxisDot[AXIS_Z][AXIS_Y] + _Src.HalfLength.z * AbsAxisDot[AXIS_X][AXIS_Y];
	r2 = _Dest.HalfLength.x * AbsAxisDot[AXIS_Y][AXIS_Z] + _Dest.HalfLength.z * AbsAxisDot[AXIS_Y][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(DirDot[AXIS_X] * AxisDot[AXIS_Z][AXIS_Z] - DirDot[AXIS_Z] * AxisDot[AXIS_X][AXIS_Z]);
	r1 = _Src.HalfLength.x * AbsAxisDot[AXIS_Z][AXIS_Z] + _Src.HalfLength.z * AbsAxisDot[AXIS_X][AXIS_Z];
	r2 = _Dest.HalfLength.x * AbsAxisDot[AXIS_Y][AXIS_Y] + _Dest.HalfLength.y * AbsAxisDot[AXIS_Y][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(DirDot[AXIS_Y] * AxisDot[AXIS_X][AXIS_X] - DirDot[AXIS_X] * AxisDot[AXIS_Y][AXIS_X]);
	r1 = _Src.HalfLength.x * AbsAxisDot[AXIS_Y][AXIS_X] + _Src.HalfLength.y * AbsAxisDot[AXIS_X][AXIS_X];
	r2 = _Dest.HalfLength.y * AbsAxisDot[AXIS_Z][AXIS_Z] + _Dest.HalfLength.z * AbsAxisDot[AXIS_Z][AXIS_Y];

	if (r > r1 + r2)
		return false;

	r = abs(DirDot[AXIS_Y] * AxisDot[AXIS_X][AXIS_Y] - DirDot[AXIS_X] * AxisDot[AXIS_Y][AXIS_Y]);
	r1 = _Src.HalfLength.x * AbsAxisDot[AXIS_Y][AXIS_Y] + _Src.HalfLength.y * AbsAxisDot[AXIS_X][AXIS_Y];
	r2 = _Dest.HalfLength.x * AbsAxisDot[AXIS_Z][AXIS_Z] + _Dest.HalfLength.z * AbsAxisDot[AXIS_Z][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(DirDot[AXIS_Y] * AxisDot[AXIS_X][AXIS_Z] - DirDot[AXIS_X] * AxisDot[AXIS_Y][AXIS_Z]);
	r1 = _Src.HalfLength.x * AbsAxisDot[AXIS_Y][AXIS_Z] + _Src.HalfLength.y * AbsAxisDot[AXIS_X][AXIS_Z];
	r2 = _Dest.HalfLength.x * AbsAxisDot[AXIS_Z][AXIS_Y] + _Dest.HalfLength.y * AbsAxisDot[AXIS_Z][AXIS_X];

	if (r > r1 + r2)
		return false;

	return true;
}

void AIMCollider::SetCallback(CollisionCallbackState _State, void(*_Func)(Ezptr<AIMCollider>, Ezptr<AIMCollider>, float))
{
	bCallback[_State] = true;
	Callback[_State] = std::bind(_Func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}
