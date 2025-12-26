#include "Target.h"

Target::~Target()
{
	for(auto& ren: renderTargets)
		if (ren)
		{
			ren->Release();
			ren = nullptr;
		}
	renderTargets.clear();
}

bool Target::Create(Device& devi, SwapC& swapC, Heap& heap)
{
	auto& desc = swapC.GetDesc();
	renderTargets.resize(desc.BufferCount);

	auto handle = heap.GetHeap()->GetCPUDescriptorHandleForHeapStart();

	auto type = heap.GetType();
	assert(type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープがRTVじゃあ無い");

	for (int i = 0; i < desc.BufferCount; i++)
	{
		auto hr = swapC.GetChain()->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
		if (FAILED(hr))
		{
			assert(false && "バックバッファ取得ー失敗ー");
			return true;
		}
		devi.GetDev()->CreateRenderTargetView(renderTargets[i], nullptr, handle);
		handle.ptr += devi.GetDev()->GetDescriptorHandleIncrementSize(type);
	}
	return false;
}

D3D12_CPU_DESCRIPTOR_HANDLE Target::GetHandle(Device& devi, Heap& heap, UINT idx)
{
	if (idx >= renderTargets.size() || !renderTargets[idx])
		assert(false && "レンダ―ターゲットの範囲外");

	auto handle = heap.GetHeap()->GetCPUDescriptorHandleForHeapStart();

	auto heapType = heap.GetType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ヒープタイプがRTVじゃあない");

	handle.ptr += idx * devi.GetDev()->GetDescriptorHandleIncrementSize(heapType);
	return handle;
}