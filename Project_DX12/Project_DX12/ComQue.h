#pragma once

#include "Device.h"

class ComQue
{
private:
	ID3D12CommandQueue* commandQ{};

public:
	ComQue() = default;
	~ComQue();

	bool Create(Device& dev);

	ID3D12CommandQueue* GetQ() { return commandQ; }
};

