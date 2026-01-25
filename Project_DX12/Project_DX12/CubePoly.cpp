#include "CubePoly.h"

CubePoly::~CubePoly()
{/*
	if(vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if(indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}*/
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
			{{-0.1f,  0.1f, -0.1f}, {1.0f, 0.0f, 0.0f, 1.0f}}, //0:左上前
			{{ 0.1f,  0.1f, -0.1f}, {0.0f, 1.0f, 0.0f, 1.0f}}, //1:右上
			{{-0.1f, -0.1f, -0.1f}, {0.0f, 0.0f, 1.0f, 1.0f}}, //2:左下
			{{ 0.1f, -0.1f, -0.1f}, {1.0f, 1.0f, 0.0f, 1.0f}}, //3:右下
			{{-0.1f,  0.1f,  0.1f}, {1.0f, 0.0f, 1.0f, 1.0f}}, //4:左上奥
			{{ 0.1f,  0.1f,  0.1f}, {0.0f, 1.0f, 1.0f, 1.0f}}, //5:右上
			{{-0.1f, -0.1f,  0.1f}, {1.0f, 1.0f, 1.0f, 1.0f}}, //6:左下
			{{ 0.1f, -0.1f,  0.1f}, {0.0f, 0.0f, 0.0f, 1.0f}}  //7:右下
		};
		auto size = sizeof(cubeVertices);

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
		memcpy_s(data, size, cubeVertices, size);
		vertexBuffer->Unmap(0, nullptr);

		vertexView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexView.SizeInBytes = size;
		vertexView.StrideInBytes = sizeof(Vertex);
	}
	/*
			筒
			2, 0, 3, 1, //前面
			7, 5,       //右面
			6, 4,		//奥面
			2, 0,		//左面

			cube?
			2, 0, 3, 1, //前面
			7, 5,       //右面
			6, 4,		//奥面
			2, 0,		//左面
			4, 5,		//上面
			6, 7		//下面
	*/
	//インデックスデータ
	{
		//cube?
		unsigned short cubeIndices[] =
		{
			6, 4, 2, 0,
			1, 2, 3, 
			6, 7, 
			4, 5,
			0, 1,
			5, 3, 7

		};
		auto size = sizeof(cubeIndices);

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
		unsigned short* data{};
		hr = indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
		if (FAILED(hr))
		{
			assert(false && "インデックスバッファのマップー失敗ー");
			return true;
		}
		memcpy_s(data, size, cubeIndices, size);
		indexBuffer->Unmap(0, nullptr);

		indexView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexView.SizeInBytes = size;
		indexView.Format = DXGI_FORMAT_R16_UINT;
	}

	return false;
}

void CubePoly::Draw(ComLis& list)
{
	list.GetList()->IASetVertexBuffers(0, 1, &vertexView);
	list.GetList()->IASetIndexBuffer(&indexView);
	list.GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	list.GetList()->DrawIndexedInstanced(16, 1, 0, 0, 0);
}