#include "Device.h"

//デストラクタ
Device::~Device()
{
	if(device)
	{
		device->Release();
		device = nullptr;
	}
}

//デバイスの作成
bool Device::Create(IDXGIAdapter4* adapter)
{
	//対応レベル
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	//使用できるLevelを調べる
	for(auto level : levels)
	{
		if(D3D12CreateDevice(
			adapter,				//アダプター ( nullptrで既定のアダプター )
			level,					//機能レベル
			IID_PPV_ARGS(&device)	//デバイス
		) == S_OK)
		{	//適正Level発見
			break;
		}
	}

	if(!device)
	{
		assert(false && "デバイスの作成ー失敗ー");
		return true;
	}

	return false;
}