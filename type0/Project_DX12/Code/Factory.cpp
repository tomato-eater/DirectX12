#include "Factory.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//デストラクタ
Factory::~Factory()
{
	if (factory)
	{
		factory->Release();
		factory = nullptr;
	}
}

//ファクトリーの作成
bool Factory::Create()
{
#if _DEBUG
	// デバッグレイヤーをオンに
	// これを行う事で、DirectXのエラー内容をより詳細に知ることができる
	ID3D12Debug* debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
		debug->EnableDebugLayer();
	}
#endif

	// ファクトリーの生成
	UINT factoryFlags = 0;
#ifdef _DEBUG
	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	auto hr = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));

	if(FAILED(hr))
	{
		assert(false && "ファクトリーの作成ー失敗ー");
		return true;
	}

	return false;
}