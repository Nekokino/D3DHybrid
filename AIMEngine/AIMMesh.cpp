#include "AIMMesh.h"
#include "AIMDevice.h"

AIMMesh::AIMMesh()
{
}


AIMMesh::~AIMMesh()
{
	for (size_t AA = 0; AA < MeshContainerVec.size(); ++AA)
	{
		for (size_t BB = 0; BB < MeshContainerVec[AA]->IBVec.size(); ++BB)
		{
			if (nullptr != MeshContainerVec[AA]->IBVec[BB]->Data)
			{
				delete MeshContainerVec[AA]->IBVec[BB]->Data;
				MeshContainerVec[AA]->IBVec[BB]->Data = nullptr;
			}

			if (nullptr != MeshContainerVec[AA]->IBVec[BB]->Buffer)
			{
				MeshContainerVec[AA]->IBVec[BB]->Buffer->Release();
				MeshContainerVec[AA]->IBVec[BB]->Buffer = nullptr;
			}

			if (nullptr != MeshContainerVec[AA]->IBVec[BB])
			{
				delete MeshContainerVec[AA]->IBVec[BB];
				MeshContainerVec[AA]->IBVec[BB] = nullptr;
			}
		}

		if (nullptr != MeshContainerVec[AA]->VB.Data)
		{
			delete MeshContainerVec[AA]->VB.Data;
			MeshContainerVec[AA]->VB.Data = nullptr;
		}

		if (nullptr != MeshContainerVec[AA]->VB.Buffer)
		{
			MeshContainerVec[AA]->VB.Buffer->Release();
			MeshContainerVec[AA]->VB.Buffer = nullptr;
		}

		if (nullptr != MeshContainerVec[AA])
		{
			delete MeshContainerVec[AA];
			MeshContainerVec[AA] = nullptr;
		}
	}

	MeshContainerVec.clear();
}


bool AIMMesh::CreateMesh(const std::string & _Name, const std::string& _ShaderName, const std::string& _InputLayoutName, int _VtxSize, int _VtxCount, D3D11_USAGE _VtxUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void * _Vtx, int _IdxSize, int _IdxCount, D3D11_USAGE _IdxUsage, DXGI_FORMAT _Format, void * _Idx)
{
	NameTag = _Name;
	ShaderName = _ShaderName;
	InputLayoutName = _InputLayoutName;

	MeshContainer* Container = new MeshContainer;

	MeshContainerVec.push_back(Container);

	if (false == CreateVertexBuffer(_VtxSize, _VtxCount, _VtxUsage, _Primitive, _Vtx))
	{
		return false;
	}

	if (false == CreateIndexBuffer(_IdxSize, _IdxCount, _IdxUsage, _Format, _Idx))
	{
		return false;
	}

	return true;
}

void AIMMesh::Render()
{
	for (size_t AA = 0; AA < MeshContainerVec.size(); ++AA)
	{
		MeshContainer* Container = MeshContainerVec[AA];

		UINT Stride = Container->VB.Size;
		UINT Offset = 0;

		// 컨텍스트에 버텍스 버퍼의 토폴로지 가져와서 세팅
		GetAIMContext->IASetPrimitiveTopology(Container->VB.Primitive);
		// 컨텍스트에 버텍스 버퍼 세팅
		// (앞의 첫번째 인자는 어차피 버텍스버퍼를 한개만 사용할거라 0 고정이고, 두번째 인자 역시 1개만 사용할 것이므로 1 고정이다)
		GetAIMContext->IASetVertexBuffers(0, 1, &Container->VB.Buffer, &Stride, &Offset);

		if (true == Container->IBVec.empty())
		{
			// 인덱스 버퍼가 없다면 그냥 깡으로 그리고
			GetAIMContext->Draw(Container->VB.Count, 0);
		}
		else
		{
			for (size_t BB = 0; BB < Container->IBVec.size(); ++BB)
			{
				IndexBuffer* Subset = Container->IBVec[BB];

				// 인덱스버퍼가 있다면 인덱스 버퍼를 컨텍스트에 세팅하고
				GetAIMContext->IASetIndexBuffer(Subset->Buffer, Subset->Format, 0);
				// 인덱스 버퍼대로 그려준다.
				GetAIMContext->DrawIndexed(Subset->Count, 0, 0);
			}
		}
	}
}

void AIMMesh::Render(size_t _Container, size_t _SubSet)
{
	MeshContainer* Container = MeshContainerVec[_Container];

	UINT Stride = Container->VB.Size;
	UINT Offset = 0;

	GetAIMContext->IASetPrimitiveTopology(Container->VB.Primitive);
	GetAIMContext->IASetVertexBuffers(0, 1, &Container->VB.Buffer, &Stride, &Offset);

	if (true == Container->IBVec.empty())
	{
		GetAIMContext->Draw(Container->VB.Count, 0);
	}
	else
	{
		IndexBuffer* Subset = Container->IBVec[_SubSet];

		GetAIMContext->IASetIndexBuffer(Subset->Buffer, Subset->Format, 0);
		GetAIMContext->DrawIndexed(Subset->Count, 0, 0);
	}
}

bool AIMMesh::CreateVertexBuffer(int _Size, int _Count, D3D11_USAGE _Usage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void * _Data)
{
	MeshContainer* Container = MeshContainerVec.back();

	D3D11_BUFFER_DESC Desc = {};

	Desc.ByteWidth = _Size * _Count;
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Desc.Usage = _Usage;

	switch (_Usage)
	{
	case D3D11_USAGE_DYNAMIC:
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	Container->VB.Size = _Size;
	Container->VB.Count = _Count;
	Container->VB.Usage = _Usage;
	Container->VB.Primitive = _Primitive;
	Container->VB.Data = new char[_Size * _Count];

	memcpy(Container->VB.Data, _Data, _Size * _Count);

	D3D11_SUBRESOURCE_DATA Data = {};

	Data.pSysMem = Container->VB.Data;

	if (FAILED(GetAIMDevice->CreateBuffer(&Desc, &Data, &Container->VB.Buffer)))
	{
		tassertmsg(true, "VertexBuffer Create Failed");
		return false;
	}

	return true;
}

bool AIMMesh::CreateIndexBuffer(int _Size, int _Count, D3D11_USAGE _Usage, DXGI_FORMAT _Format, void * _Data)
{
	MeshContainer* Container = MeshContainerVec.back();

	IndexBuffer* IB = new IndexBuffer;

	Container->IBVec.push_back(IB);

	D3D11_BUFFER_DESC Desc = {};

	Desc.ByteWidth = _Size * _Count;
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Desc.Usage = _Usage;

	switch (_Usage)
	{
	case D3D11_USAGE_DYNAMIC:
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	IB->Size = _Size;
	IB->Count = _Count;
	IB->Usage = _Usage;
	IB->Format = _Format;
	IB->Data = new char[_Size * _Count];

	memcpy(IB->Data, _Data, _Size * _Count);

	D3D11_SUBRESOURCE_DATA Data = {};

	Data.pSysMem = IB->Data;

	if (FAILED(GetAIMDevice->CreateBuffer(&Desc, &Data, &IB->Buffer)))
	{
		tassertmsg(true, "IndexBuffer Create Failed");
		return false;
	}

	return true;
}