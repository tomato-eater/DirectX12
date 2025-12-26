#pragma once

#include "Fac_Ada.h"

class Device
{
private:
	ID3D12Device* device;

public:
	Device() = default;
	~Device();

	bool Create(Fac_Ada& f_a);

	ID3D12Device* GetDev() { return device; }
};

