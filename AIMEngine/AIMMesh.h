#pragma once

#include "RefCounter.h"

typedef struct Engine_DLL _tagVertexBuffer
{
	ID3D11Buffer* Buffer;
	void* Data;
	int Size;
	int Count;
	D3D11_USAGE Usage;
	D3D11_PRIMITIVE_TOPOLOGY Primitive;
}VertexBuffer, *PVertexBuffer;

typedef struct Engine_DLL _tagIndexBuffer
{
	ID3D11Buffer* Buffer;
	void* Data;
	int Size;
	int Count;
	D3D11_USAGE Usage;
	DXGI_FORMAT Format;
}IndexBuffer, *PIndexBuffer;

// 메쉬 컨테이너 하나에는 한개의 버텍스버퍼와 여러개의 인덱스 버퍼로 이루어지게 했다.
typedef struct Engine_DLL _tagMeshContainer
{
	VertexBuffer VB;
	std::vector<IndexBuffer*> IBVec;
}MeshContainer, *PMeshContainer;

class Engine_DLL AIMMesh : public RefCounter
{
public:
	friend class ResourceManager;

private:
	std::vector<MeshContainer*> MeshContainerVec;

public:
	bool CreateMesh(const std::string& _Name, int _VtxSize, int _VtxCount, D3D11_USAGE _VtxUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void* _Vtx,
		int _IdxSize = 0, int _IdxCount = 0, D3D11_USAGE _IdxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT _Format = DXGI_FORMAT_UNKNOWN, void* _Idx = nullptr);

	// 모든 컨테이너의 모든 서브셋을 다 그리는 렌더
	void Render();
	// 특정 컨테이너의 특정 서브셋을 그리는 렌더
	void Render(int _Container, int _SubSet = 0);

private:
	bool CreateVertexBuffer(int _Size, int _Count, D3D11_USAGE _Usage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void* _Data);
	bool CreateIndexBuffer(int _Size, int _Count, D3D11_USAGE _Usage, DXGI_FORMAT _Format, void* _Data);

private:
	AIMMesh();
	~AIMMesh();
};