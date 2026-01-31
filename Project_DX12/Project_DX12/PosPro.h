#pragma once

#include "Target.h"
#include "Device.h"
#include "Heap.h"

class PosPro
{
private:
	ID3D12Resource* screenResource;
	ID3D12DescriptorHeap* rtvHeap;
	ID3D12DescriptorHeap* srvHeap;

	ID3D12Resource* vectorBuffer;
	D3D12_VERTEX_BUFFER_VIEW bufferView;

public:
	PosPro() = default;
	~PosPro();

	bool CreateResource(Target&, Device&, Heap&);

};

