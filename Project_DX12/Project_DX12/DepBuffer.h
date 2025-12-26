#pragma once

#include "Device.h"
#include "Heap.h"

class DepBuffer
{
private:
	ID3D12Resource* depthBuffer{};
	D3D12_CPU_DESCRIPTOR_HANDLE handle{};

public:
	DepBuffer() = default;
	~DepBuffer();

	bool Create(Device& devi, Heap& heap, int wi, int hi);

	ID3D12Resource* GetBuf() { return depthBuffer; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() { return handle; }
};

