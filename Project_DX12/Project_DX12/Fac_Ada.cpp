#include "Fac_Ada.h"

#pragma comment(lib, "dxgi.lib")

Fac_Ada::~Fac_Ada()
{
	if (factory)
	{
		factory->Release();
		factory = nullptr;
	}
	if (adapter)
	{
		adapter->Release();
		adapter = nullptr;
	}
}

bool Fac_Ada::Create()
{
	//ファクトリー
	{
		UINT factFlag = 0;
#if _DEBUG
		factFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif
		auto hr = CreateDXGIFactory2(factFlag, IID_PPV_ARGS(&factory));
		if (FAILED(hr))
		{
			assert(false && "ファクトリー作成ー失敗ー");
			return true;
		}
	}

	//アダプター
	{
		int idx = 0;
		IDXGIAdapter1* ada{};
		while (DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(idx, &ada))
		{
			DXGI_ADAPTER_DESC1 desc{};
			ada->GetDesc1(&desc);
			idx++;
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				ada->Release();
				continue;
			}
			if (FAILED(D3D12CreateDevice(ada, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
			{
				ada->Release();
				continue;
			}
			adapter = ada;
			break;
		}

		if (!adapter)
		{
			assert(false && "アダプター取得ー失敗ー");
			return true;
		}
	}
	return false;
}