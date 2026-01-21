#include "CubePoly.h"

CubePoly::~CubePoly()
{
	if(vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if(indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
}

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

void CubePoly::SetHeap(D3D12_HEAP_PROPERTIES& heap)
{
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap.CreationNodeMask = 1;
	heap.VisibleNodeMask = 1;
}

void CubePoly::SetDesc(D3D12_RESOURCE_DESC& desc, UINT size)
{
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = size;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;
}

bool CubePoly::Create(Device& devi)
{
	//頂点データ
	{
		Vertex cubeVertices[] =
		{
			{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
			{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
			{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
			{{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},
			{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f, 1.0f}}
		};
		auto size = sizeof(cubeVertices);
		D3D12_HEAP_PROPERTIES heap{};
		D3D12_RESOURCE_DESC desc{};
		SetHeap(heap);
		SetDesc(desc, size);
		auto hr = devi.GetDev()->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		if (FAILED(hr))
		{
			assert(false && "頂