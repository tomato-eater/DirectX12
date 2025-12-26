#pragma once

#include <vector>
using namespace std;

#include "Device.h"
#include "SwapC.h"
#include "Heap.h"

class Target
{
private:
	vector<ID3D12Resource*> renderTargets{};

public:
	Target() = default;
	~Target();

	bool Create(Device& devi, SwapC& swapC, Heap& heap);

	ID3D12Resource* GetTarget(int idx) { return renderTargets[idx]; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(Device& devi, Heap& heap, UINT idx);
};

