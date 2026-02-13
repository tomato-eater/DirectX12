#include "Device.h"

#include <cassert>

#include "DXGI.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

//デバイスの作成
bool Device::Create()
{
	//まずファクトリー
	if (DXGI::Ins().CreateFactory()) {
		assert(false && "ファクトリー作成ー失敗ー");
		return true;
	}

	if (DXGI::Ins().CreateAdapter()) {
		assert(false && "アダプター作成ー失敗ー");
		return true;
	}

	//対応レベル
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
	};

	//使用できるLevelを調べる
	for(auto level : levels)
	{
		if(D3D12CreateDevice(
			DXGI::Ins().Adapter(),		//アダプター ( nullptrで既定のアダプター )
			level,						//機能レベル
			IID_PPV_ARGS(&device)		//デバイス
		) == S_OK)
		{	//適正Level発見
			break;
		}
	}
	return false;
}