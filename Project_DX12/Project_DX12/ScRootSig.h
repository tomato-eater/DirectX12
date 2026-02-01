#pragma once

#include "Device.h"

class ScRootSig
{
private:
	ID3D12RootSignature* rootSignature{};

public:
	ScRootSig() = default;
	~ScRootSig();

	bool Create(Device& devi);

	ID3D12RootSignature* GetSign() { return rootSignature; }
};

