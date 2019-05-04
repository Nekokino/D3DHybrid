#include "RenderState.h"


RenderState::RenderState()
{
}


RenderState::~RenderState()
{
	if (State != nullptr)
	{
		State->Release();
		State = nullptr;
	}

	if (OldState != nullptr)
	{
		OldState->Release();
		OldState = nullptr;
	}
}

RenderStateType RenderState::GetRenderStateType() const
{
	return Type;
}