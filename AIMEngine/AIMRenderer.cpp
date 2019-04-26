#include "AIMRenderer.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "AIMDevice.h"


AIMRenderer::AIMRenderer()
{
	CT = CT_RENDERER;
}

AIMRenderer::AIMRenderer(const AIMRenderer & _Other)
{
	*this = _Other;
}


AIMRenderer::~AIMRenderer()
{
	if (InputLayout != nullptr)
	{
		InputLayout->Release();
		InputLayout = nullptr;
	}
}

void AIMRenderer::SetMesh(const std::string & _Key)
{
	Mesh = ResourceManager::FindMesh(_Key);

	if (Mesh != nullptr)
	{
		SetShader(Mesh->GetShaderName());
		SetInputLayout(Mesh->GetInputLayoutName());
	}
}

void AIMRenderer::SetShader(const std::string & _Key)
{
	Shader = ShaderManager::FindShader(_Key);
}

void AIMRenderer::SetInputLayout(const std::string & _Key)
{
	InputLayout = ShaderManager::FindInputLayout(_Key);
}

void AIMRenderer::Start()
{
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
	return 0;
}

int AIMRenderer::Render(float _Time)
{
	GetAIMContext->IASetInputLayout(InputLayout);

	Shader->SetShader();

	size_t Container = Mesh->GetContainerCount();

	for (size_t AA = 0; AA < Container; ++AA)
	{
		size_t Subset = Mesh->GetSubsetCount();
		for (size_t BB = 0; BB < Subset; ++BB)
		{
			Mesh->Render(AA, BB);
		}
	}

	return 0;
}

AIMRenderer* AIMRenderer::Clone() const
{
	return new AIMRenderer(*this);
}
