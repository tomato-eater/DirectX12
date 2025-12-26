#include "ConBuffer.h"

ConBuffer::~ConBuffer()
{/*
	if (constantBuffer)
	{
		constantBuffer->Release();
		constantBuffer = nullptr;
	}*/
}

bool ConBuffer::Create(Device& devi, Heap& heap, UINT bufferSize, UINT idx)
{
	auto size = (sizeof(bufferSize) + 255) & ~255;

	D3D12_HEAP_PROPERTIES hProp{};
	hProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC rDesc{};
	rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	rDesc.Width = size;
	rDesc.Height = 1;
	rDesc.DepthOrArraySize = 1;
	rDesc.MipLevels = 1;
	rDesc.Format = DXGI_FORMAT_UNKNOWN;
	rDesc.SampleDesc.Count = 1;
	rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto hr = devi.GetDev()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constantBuffer));
	if (FAILED(hr))
	{
		assert(false && "コンスタントバッファ作成ー失敗ー");
		return true;
	}

	auto hType = heap.GetType();
	if (hType != D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	{
		assert(false && "ディスクリプタヒープタイプが違う");
		return true;
	}

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = size;

	UINT cbvSize = devi.GetDev()->GetDescriptorHandleIncrementSize(heap.GetType());

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHand = heap.GetHeap()->GetCPUDescriptorHandleForHeapStart();
	cpuHand.ptr += idx * cbvSize;
	devi.GetDev()->CreateConstantBufferView(&cbvDesc, cpuHand);

	gpuHandle = heap.GetHeap()->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += idx * cbvSize;

	return false;
}