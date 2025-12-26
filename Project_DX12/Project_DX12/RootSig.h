#pragma once

#include "Device.h"

class RootSig
{
private:
	ID3D12RootSignature* rootSignature{};

public:
	RootSig() = default;
	~RootSig();

	bool Create(Device& devi);

	ID3D12RootSignature* GetSign() { return rootSignature; }
};

