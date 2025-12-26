#include "SwapC.h"

SwapC::~SwapC()
{
	if (chain)
	{
		chain->Release();
		chain = nullptr;
	}
}

bool SwapC::Create(Window& win, Fac_Ada& f_a, ComQue& comQ, int wi, int hi)
{
	chainDesc = {};
	chainDesc.BufferCount = 2;
	chainDesc.Width = wi;
	chainDesc.Height = hi;
	chainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.SampleDesc.Count = 1;

	IDXGISwapChain1* tempChain{};
	auto hr = f_a.GetFac()->CreateSwapChainForHwnd(comQ.GetQ(), win.GetHandle(), &chainDesc, nullptr, nullptr, &tempChain);
	if (FAILED(hr))
	{
		assert(false && "スワップチェイン作成ー失敗ー");
		return true;
	}

	hr = tempChain->QueryInterface(IID_PPV_ARGS(&chain));
	tempChain->Release();
	if (FAILED(hr))
	{
		assert(false && "スワップチェインアップグレードー失敗ー");
		return true;
	}

	return false;
}