#pragma once
#include <cassert>

#include <d3d12.h>
#include <dxgi1_6.h>

//デバイスクラス
class Device
{
private:
	ID3D12Device* device; //デバイスインターフェース
	D3D_FEATURE_LEVEL featureLevel; //機能レベル

public:
	Device() = default; //コンストラクタ
	~Device();			//デストラクタ

	//デバイスの作成
	bool Create(IDXGIAdapter4* adapter = nullptr);

	//デバイスインターフェースの取得
	ID3D12Device* Get() const { return device; }
};

