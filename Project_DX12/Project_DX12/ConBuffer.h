#pragma once

#include "Device.h"
#include "Heap.h"

#include "PosPro.h"

class ConBuffer
{
private:
	ID3D12Resource* constantBuffer{};
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

public:
	ConBuffer() = default;
	~ConBuffer();

	//bool Create(Device& devi, Heap& heap, UINT bufferSize, UINT idx);
	bool Create(Device& devi, PosPro& heap, UINT bufferSize, UINT idx);

	ID3D12Resource* GetBuf() { return constantBuffer; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetHand() { return gpuHandle; }
};

