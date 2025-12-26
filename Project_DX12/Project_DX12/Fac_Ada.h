#pragma once
#include <cassert>

#include <d3d12.h>
#include <dxgi1_4.h>

class Fac_Ada
{
private:
	IDXGIFactory4* factory;
	IDXGIAdapter1* adapter;

public:
	Fac_Ada() = default;
	~Fac_Ada();

	bool Create();

	IDXGIFactory4* GetFac() { return factory; }
	IDXGIAdapter1* GetAda() { return adapter; }
};

