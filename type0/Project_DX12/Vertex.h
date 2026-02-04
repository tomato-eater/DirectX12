#pragma once

#include <DirectXMath.h>
#include <d3d12.h>

class VertexTemp
{
public:
	VertexTemp() = default;
	~VertexTemp() = default;
	//座標と色を記憶するやつ
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
	};

	//ヒープを準備する便利なやつ
	static D3D12_HEAP_PROPERTIES SetHeap()
	{
		D3D12_HEAP_PROPERTIES heap;
		heap.Type = D3D12_HEAP_TYPE_UPLOAD;
		heap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap.CreationNodeMask = 1;
		heap.VisibleNodeMask = 1;

		return heap;
	}

	//デスクを準備する便利なやつ
	static D3D12_RESOURCE_DESC SetDesc(UINT size)
	{
		D3D12_RESOURCE_DESC desc;
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

		return desc;
	}
};