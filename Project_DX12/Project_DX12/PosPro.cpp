#include "PosPro.h"

PosPro::~PosPro()
{
	if (screenResource) {
		screenResource->Release();
		screenResource = nullptr;
	}
	if (rtvHeap) {
		rtvHeap->Release();
		rtvHeap = nullptr;
	}
	if (srvHeap) {
		srvHeap->Release();
		srvHeap = nullptr;
	}
}

bool PosPro::Create(Target& tar, Device& devi, Heap& heap)
{
	{	///リソースの作成
		auto buff = tar.GetTarget(0);
		auto resDesc = buff->GetDesc();

		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProp.CreationNodeMask = 1;
		heapProp.VisibleNodeMask = 1;

		float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		D3D12_CLEAR_VALUE clearValue{};
		clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		memcpy_s(clearValue.Color, sizeof(clearColor), clearColor, sizeof(clearColor));

		auto hr = devi.GetDev()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_PRESENT, &clearValue, IID_PPV_ARGS(&screenResource));
		if (FAILED(hr)) {
			assert(false && "画面転送用リソース作成ー失敗ー");
			return true;
		}

		auto heapDesc = heap.GetHeap()->GetDesc();
		heapDesc.NumDescriptors = 1;
		hr = devi.GetDev()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));
		if (FAILED(hr)) {
			assert(false && "RTVヒープ作成ー失敗ー");
			return true;
		}

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		devi.GetDev()->CreateRenderTargetView(screenResource, &rtvDesc, rtvHeap->GetCPUDescriptorHandleForHeapStart());

		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NodeMask = 0;

		hr = devi.GetDev()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&srvHeap));
		if (FAILED(hr)) {
			assert(false && "SRVヒープ作成ー失敗ー");
			return true;
		}

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		devi.GetDev()->CreateShaderResourceView(screenResource, &srvDesc, srvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	{	///頂点バッファの作成
		struct ScreenVertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 uv;
		};

		ScreenVertex vertices[] =
		{
			{{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f}},
			{{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}},
			{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
			{{ 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}
		};
		auto size = sizeof(vertices);

		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProp.CreationNodeMask = 1;
		heapProp.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Alignment = 0;
		resDesc.Width = size;
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.Format = DXGI_FORMAT_UNKNOWN;
		resDesc.SampleDesc.Count = 1;
		resDesc.SampleDesc.Quality = 0;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		
		auto hr = devi.GetDev()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		if(FAILED(hr)) {
			assert(false && "画面転送用頂点バッファ作成ー失敗ー");
			return true;
		}

		ScreenVertex* vertMap = nullptr;
		hr = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
		if (FAILED(hr)) {
			assert(false && "画面転送用頂点バッファマップー失敗ー");
			return true;
		}
		memcpy_s(vertMap, size, vertices, size);
		vertexBuffer->Unmap(0, nullptr);

		bufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		bufferView.SizeInBytes = size;
		bufferView.StrideInBytes = sizeof(ScreenVertex);
	}

	return false;
}

void PosPro::Draw(ComLis& comLis)
{
	comLis.GetList()->IASetVertexBuffers(0, 1, &bufferView);
	comLis.GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	comLis.GetList()->DrawInstanced(4, 1, 0, 0);
}