#include "DXGI.h"
#include <cassert>

#include <d3d12.h>
#pragma comment(lib, "dxgi.lib")

//ファクトリー　作成
bool DXGI::CreateFactory()
{
	//デバッグレイヤー
#if _DEBUG
	ID3D12Debug* debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
		debug->EnableDebugLayer();
#endif

	UINT fFlag = 0;
#if _DEBUG
	fFlag |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	if (CreateDXGIFactory2(
		fFlag,					//
		IID_PPV_ARGS(&factory)))//
	{
		assert(false && "ファクトリ作成ー失敗ー");
		return true;
	}
	return false;
}

//アダプター　作成
bool DXGI::CreateAdapter()
{
	int select = 0;
	IDXGIAdapter1* ada{};
	//アダプターの選択
	while (factory->EnumAdapters1(select, &ada) != DXGI_ERROR_NOT_FOUND) {
		DXGI_ADAPTER_DESC1 desc{};
		ada->GetDesc1(&desc);
		select++;
		//ソフトウェアアダプター　除外
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			ada->Release();
			continue;
		}
		//3Dが動かない　除外
		if (FAILED(D3D12CreateDevice(ada, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
			ada->Release();
			continue;
		}
		adapter = ada;
		break;
	}
	if (!adapter) {
		assert("アダプタ取得ー失敗ー");
		return true;
	}
	return false;
}