#pragma once

#include "Target.h"
#include "Device.h"
#include "Heap.h"

#include <DirectXMath.h>

class PosPro
{
private:
	ID3D12Resource* screenResource;
	ID3D12DescriptorHeap* rtvHeap;
	ID3D12DescriptorHeap* srvHeap;

	ID3D12Resource* vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW bufferView;

public:
	PosPro() = default;
	~PosPro();

	bool Create(Target&, Device&, Heap&);

	ID3D12Resource* GetResource() { return screenResource; }
	ID3D12DescriptorHeap* GetRtvHeap() { return rtvHeap; }
	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap; }
};

/*
4
14
17

*/
