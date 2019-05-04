#pragma once

#include "RefCounter.h"
class Engine_DLL RenderState : public RefCounter
{
	friend class RenderManager;

protected:
	ID3D11DeviceChild* State = nullptr;
	ID3D11DeviceChild* OldState = nullptr;
	RenderStateType Type;

public:
	RenderStateType GetRenderStateType() const;

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;

protected:
	RenderState();
	~RenderState();
};

