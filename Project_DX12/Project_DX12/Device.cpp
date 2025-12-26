#include "Device.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

Device::~Device()
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}
}

bool Device::Create(Fac_Ada& f_a)
{
	auto hr = D3D12CreateDevice(f_a.GetAda(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
	if (FAILED(hr))
	{
		assert(false && "デバイス作成ー失敗ー");
		return true;
	}
	return false;
}