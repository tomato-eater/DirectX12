#include "ConsBuffer.h"
#include <cassert>

#include "Device.h"
#include "Heap.h"

//コンスタントバッファの作成s
bool ConsBuffer::Create(UINT bufferSize)
{
	auto type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	const auto size = (sizeof(bufferSize) + 0xff) & ~0xff;

	//ヒープ状態
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	//バッファリソース作成
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Alignment = 0;
	resDesc.Width = size;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc = { 1,0 };
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	if (Device::Ins().Get()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&consbuffer)) != S_OK)	{
		assert(false && "コンスタントバッファの作成ー失敗ー");
		return true;
	}

	//ビューの作成
	const auto heapIdx = HeapReader::Ins().GetNum(type);
	if (heapIdx < 0) {
		assert(false && "ヒープの確保ー失敗ー");
		return true;
	}
	auto heap = HeapReader::Ins().GetHeap(type);
	
	//ビューの設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = consbuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = size;

	//ディスクリプタサイズ　ハンドルサイズ　取得　
	UINT cbvDescSize = Device::Ins().Get()->GetDescriptorHandleIncrementSize(type);
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
	cpuHandle.ptr += heapIdx * cbvDescSize;

	//紐づける
	Device::Ins().Get()->CreateConstantBufferView(&cbvDesc, cpuHandle);

	handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += heapIdx * cbvDescSize;

	return false;
}