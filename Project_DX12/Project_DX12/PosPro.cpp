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

bool PosPro::CreateResource(Target& tar, Device& devi, Heap& heap)
{
	auto buff = tar.GetTarget(0);
	auto resDesc = buff->GetDesc();

	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	for (int i = 0; i < 4; i++)
		clearValue.Color[i] = clearColor[i];

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

	return false;
}

/*
4
14
17

*/