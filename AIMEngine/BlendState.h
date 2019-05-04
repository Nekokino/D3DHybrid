#pragma once
#include "RenderState.h"
class Engine_DLL BlendState : public RenderState
{
	friend class RenderManager;

public:
	virtual void SetState() override;
	virtual void ResetState() override;
private:
	BlendState();
	~BlendState();
};

