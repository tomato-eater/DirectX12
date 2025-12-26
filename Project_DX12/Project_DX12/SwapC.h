#pragma once

#include "Window.h"
#include "Fac_Ada.h"
#include "ComQue.h"

class SwapC
{
private:
	IDXGISwapChain3* chain{};
	DXGI_SWAP_CHAIN_DESC1 chainDesc{};

public:
	SwapC() = default;
	~SwapC();

	bool Create(Window& win, Fac_Ada& f_a, ComQue& comQ, int wi, int hi);

	IDXGISwapChain3* GetChain() { return chain; }
	DXGI_SWAP_CHAIN_DESC1& GetDesc() { return chainDesc; }
};

