#pragma once

#include "Device.h"

class ComAll
{
private:
	ID3D12CommandAllocator* allocator{};
	D3D12_COMMAND_LIST_TYPE type{};

public:
	ComAll() = default;
	~ComAll();

	bool Create(Device& devi, D3D12_COMMAND_LIST_TYPE setType);

	void Reset() { allocator->Reset(); }

	ID3D12CommandAllocator* GetAll() { return allocator; }
	D3D12_COMMAND_LIST_TYPE GetType() { return type; }
};

