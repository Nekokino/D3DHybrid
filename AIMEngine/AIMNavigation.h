#pragma once
#include "AIMComponent.h"

class AIMTransform;
class Engine_DLL AIMNavigation : public AIMComponent
{
	friend class AIMObject;
private:
	bool OnCellYPosition = true;
	bool MouseMove = false;
	bool Find = false;
	std::list<Vec3> PathList;
	Ezptr<AIMTransform> Target;
	Vec3 Way;
	Vec3 TargetPos;
	float TargetDetectTime = 0.0f;

public:
	void SetCellYPosition(bool _Enable);
	void SetMouseMove(bool _Enable);
	void SetAIFindPath(bool _Enable);
	void SetTarget(Ezptr<AIMObject> _Target);

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _Time);
	virtual int Update(float _Time);
	virtual int LateUpdate(float _Time);
	virtual int Collision(float _Time);
	virtual int PrevRender(float _Time);
	virtual int Render(float _Time);
	virtual AIMNavigation* Clone() const;

protected:
	AIMNavigation();
	AIMNavigation(const AIMNavigation& _Other);
	~AIMNavigation();
};

