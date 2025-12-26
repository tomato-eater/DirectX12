#include "DeltaPoly.h"

DeltaPoly::~DeltaPoly()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if (indexBuffer)
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

void DeltaPoly::SetHeap(D3D12_HEAP_PROPERTIES& heap)
{
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap.CreationNodeMask = 1;
	heap.VisibleNodeMask = 1;
}

void DeltaPoly::SetDesc(D3D12_RESOURCE_DESC& desc, UINT size)
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

bool DeltaPoly::Create(Device& devi)
{
	//頂点データ
	{
		Vertex deltaVertices[] =
		{
			{{ 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
			{{ 1.0f, -1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
			{{-1.0f, -1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
		};
		auto size = sizeof(deltaVertices);

		D3D12_HEAP_PROPERTIES heap{};
		D3D12_RESOURCE_DESC desc{};

		SetHeap(heap);
		SetDesc(desc, size);

		auto hr = devi.GetDev()->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		if (FAILED(hr))
		{
			assert(false && "頂点バッファ作成ー失敗ー");
			return true;
		}

		Vertex* data{};

		hr = vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
		if (FAILED(hr))
		{
			assert(false && "頂点バッファのマップー失敗ー");
			return true;
		}

		memcpy_s(data, size, deltaVertices, size);

		vertexBuffer->Unmap(0, nullptr);

		vertexView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexView.SizeInBytes = size;
		vertexView.StrideInBytes = sizeof(Vertex);
	}

	//インデックスバッファ
	{
		uint16_t deltaIndices[] = { 0, 1, 2 };

		auto size = sizeof(deltaIndices);

		D3D12_HEAP_PROPERTIES heap{};
		D3D12_RESOURCE_DESC desc{};

		SetHeap(heap);
		SetDesc(desc, size);

		auto hr = devi.GetDev()->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBuffer));
		if (FAILED(hr))
		{
			assert(false && "インデックスバッファ作成ー失敗ー");
			return true;
		}

		uint16_t* data{};

		hr = indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
		if (FAILED(hr))
		{
			assert(false && "インデックスバッファのマップー失敗ー");
			return true;
		}

		memcpy_s(data, size, deltaIndices, size);

		indexBuffer->Unmap(0, nullptr);

		indexView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexView.SizeInBytes = size;
		indexView.Format = DXGI_FORMAT_R16_UINT;
	}

	return false;
}

void DeltaPoly::Draw(ComLis& comLis)
{
	comLis.GetList()->IASetVertexBuffers(0, 1, &vertexView);
	comLis.GetList()->IASetIndexBuffer(&indexView);
	comLis.GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	comLis.GetList()->DrawIndexedInstanced(3, 1, 0, 0, 0);
}