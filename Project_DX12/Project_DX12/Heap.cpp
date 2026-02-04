#include "Heap.h"

Heap::~Heap()
{
	if (heap)
	{
		heap->Release();
		heap = nullptr;
	}
}

bool Heap::Create(Device& devi, D3D12_DESCRIPTOR_HEAP_TYPE setType, UINT num, bool shader)
{
	type = setType;
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Type = type;
	desc.NumDescriptors = num;
	desc.Flags = shader ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	
	auto hr = devi.GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		assert(false && "ディスクリプタヒープ生成ー失敗ー");
		return true;
	}

	maxCount = num;
	for (int i = 0; i < maxCount; i++)
		freeList.push_back(i);
	
	return false;
}