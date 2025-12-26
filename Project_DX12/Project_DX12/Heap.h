#pragma once

#include "Device.h"

class Heap
{
private: 
	ID3D12DescriptorHeap* heap{};
	D3D12_DESCRIPTOR_HEAP_TYPE type{};

public:
	Heap() = default;
	~Heap();

	bool Create(Device& devi, D3D12_DESCRIPTOR_HEAP_TYPE setTypet, UINT num, bool shader = false);

	ID3D12DescriptorHeap* GetHeap() { return heap; }
	D3D12_DESCRIPTOR_HEAP_TYPE GetType(){ return type; }
};

