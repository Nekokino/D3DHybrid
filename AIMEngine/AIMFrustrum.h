#pragma once

#include "EngineHeader.h"

enum FrustrumDir
{
	FD_LEFT,
	FD_RIGHT,
	FD_TOP,
	FD_BOTTOM,
	FD_NEAR,
	FD_FAR,
	FD_END,
};
class Engine_DLL AIMFrustrum
{
	friend class AIMCamera;

private:
	Vec3 Pos[8];
	Vec4 Plane[FD_END];

public:
	void Update(const Matrix& _InvVP);
	bool FrustrumInPoint(const Vec3& _Pos);
	bool FrustrumInSphere(const Vec3& _Center, float _Radius);

private:
	AIMFrustrum();
	~AIMFrustrum();
};

