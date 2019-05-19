#pragma once
#include "AIMComponent.h"

#ifdef _DEBUG
#include "RenderState.h"
#include "AIMShader.h"
#include "AIMMesh.h"
#endif

struct _tagCollisionProfile;
class Engine_DLL AIMCollider : public AIMComponent
{
	friend class AIMObject;

#ifdef _DEBUG
protected:
	Ezptr<AIMMesh> Mesh;
	Ezptr<AIMShader> Shader;
	ID3D11InputLayout* Layout = nullptr;
	Ezptr<RenderState> WireFrame;
	Vec4 Color;
	TransformConstBuffer CBuffer;
#endif

protected:
	ColliderType ColType;
	unsigned int SerialNumber = UINT_MAX;
	Vec3 SectionMin;
	Vec3 SectionMax;
	_tagCollisionProfile* Profile = nullptr;
	unsigned int ChannelIndex = 0;
	unsigned int* PrevNumber = nullptr;
	unsigned int PrevSize = 0;
	unsigned int PrevCapacity = 0;
	unsigned int* CollisionList = nullptr;
	unsigned int CollisionSize = 0;
	unsigned int CollisionCapacity = 0;

	std::function<void(Ezptr<AIMCollider>, Ezptr<AIMCollider>, float)> Callback[CCS_END];
	bool bCallback[CCS_END] = {};

public:
	ColliderType GetColType() const
	{
		return ColType;
	}

	unsigned int GetSerialNumber() const
	{
		return SerialNumber;
	}

	Vec3 GetSectionMin() const
	{
		return SectionMin;
	}

	Vec3 GetSectionMax() const
	{
		return SectionMax;
	}

	_tagCollisionProfile* GetCollisionProfile() const;
	unsigned int GetCollisionChannelIndex() const;

public:
	void SetSerialNumber(unsigned int _Num)
	{
		SerialNumber = _Num;
	}

	void ClearState();
	void AddSerialNumber(unsigned int _SerialNumber);
	void DeleteSerialNumber(unsigned int _SerialNumber);
	void AddCollisionList(unsigned int _SerialNumber);
	void DeleteCollisionList(unsigned int _SerialNumber);
	void ClearCollisionList();
	bool CheckCollisionList(unsigned int _SerialNumber);
	void Call(CollisionCallbackState _State, Ezptr<AIMCollider> _Dest, float _Time);

	void SetChannel(const std::string& _Name);
	void SetProfile(const std::string& _Name);



public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMCollider* Clone() const;
	
	virtual bool Collision(Ezptr<AIMCollider> _Dest) = 0;

public:
	bool CollisionSp2Sp(const SphereInfo& _Src, const SphereInfo& _Dest);
	bool CollisionOBB2OBB(const OBBInfo& _Src, const OBBInfo& _Dest);

public:
	void SetCallback(CollisionCallbackState _State, void(*_Func)(Ezptr<AIMCollider>, Ezptr<AIMCollider>, float));;

	template<typename T>
	void SetCallBack(CollisionCallbackState _State, T* _Obj, void(T::*_Func)(Ezptr<AIMCollider>, Ezptr<AIMCollider>, float))
	{
		bCallback[_State] = true;
		Callback[_State] = std::bind(_Func, _Obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

protected:
	AIMCollider();
	AIMCollider(const AIMCollider& _Other);
	virtual ~AIMCollider() = 0;
};

