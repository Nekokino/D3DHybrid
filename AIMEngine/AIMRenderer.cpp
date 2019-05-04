#include "AIMRenderer.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "AIMDevice.h"
#include "AIMTransform.h"
#include "AIMMaterial.h"
#include "AIMObject.h"
#include "RenderManager.h"
#include "AIMLight.h"

AIMRenderer::AIMRenderer()
{
	CT = CT_RENDERER;
	memset(RenderStateList, 0, sizeof(Ezptr<RenderState>) * RS_END);
}

AIMRenderer::AIMRenderer(const AIMRenderer & _Other) : Shader(_Other.Shader), Mesh(_Other.Mesh), InputLayout(_Other.InputLayout), AIMComponent(_Other), Material(_Other.Material)
{
	for (size_t i = 0; i < RS_END; i++)
	{
		RenderStateList[i] = _Other.RenderStateList[i];
	}
}


AIMRenderer::~AIMRenderer()
{
	
}

void AIMRenderer::SetMesh(const std::string & _Key)
{
	Mesh = ResourceManager::FindMesh(_Key);

	if (Mesh != nullptr)
	{
		SetShader(Mesh->GetShaderName());
		SetInputLayout(Mesh->GetInputLayoutName());
	}

	Transform->SetLocalRelativeView(Mesh->GetView());
}

void AIMRenderer::SetShader(const std::string & _Key)
{
	Shader = ShaderManager::FindShader(_Key);
}

void AIMRenderer::SetInputLayout(const std::string & _Key)
{
	InputLayout = ShaderManager::FindInputLayout(_Key);
}

void AIMRenderer::SetRenderState(const std::string & _Key)
{
	Ezptr<RenderState> State = RenderManager::FindRenderState(_Key);

	if (State == nullptr)
	{
		return;
	}

	RenderStateType Type = State->GetRenderStateType();

	RenderStateList[Type] = State;
}

void AIMRenderer::Start()
{
	Material = FindComponent<AIMMaterial>(CT_MATERIAL);
}

bool AIMRenderer::Init()
{
	return true;
}

int AIMRenderer::Input(float _Time)
{
	return 0;
}

int AIMRenderer::Update(float _Time)
{
	return 0;
}

int AIMRenderer::LateUpdate(float _Time)
{
	return 0;
}

int AIMRenderer::Collision(float _Time)
{
	return 0;
}

int AIMRenderer::PrevRender(float _Time)
{
	Ezptr<AIMLight> Light = RenderManager::GetFirstLight();

	if (Light != nullptr)
	{
		Light->SetShader();
	}

	return 0;
}

int AIMRenderer::Render(float _Time)
{
	for (size_t i = 0; i < RS_END; i++)
	{
		if (RenderStateList[i] != nullptr)
		{
			RenderStateList[i]->SetState();
		}
	}

	GetAIMContext->IASetInputLayout(InputLayout);

	Shader->SetShader();

	size_t Container = Mesh->GetContainerCount();

	for (int AA = 0; AA < Container; ++AA)
	{
		size_t Subset = Mesh->GetSubsetCount();
		for (int BB = 0; BB < Subset; ++BB)
		{
			if (Material != nullptr)
			{
				Material->SetShader(AA, BB);
			}

			Mesh->Render(AA, BB);
		}
	}

	for (size_t i = 0; i < RS_END; i++)
	{
		if (RenderStateList[i] != nullptr)
		{
			RenderStateList[i]->ResetState();
		}
	}

	return 0;
}

AIMRenderer* AIMRenderer::Clone() const
{
	return new AIMRenderer(*this);
}
