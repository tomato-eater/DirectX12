#pragma once

#include "Device.h"
#include "ComQue.h"

class Fence
{
private:
	ID3D12Fence* fence{};
	HANDLE event{};
	UINT value[2] = {};
	UINT nextV = 1;

public:
	Fence() = default;
	~Fence();

	bool Create(Device& devi);

	void Check(UINT idx);

	ID3D12Fence* GetFens() { return fence; }

	void SetNext(ComQue& comQue, UINT idx);
};

