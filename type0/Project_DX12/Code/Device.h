#pragma once

#include <d3d12.h>
#include <wrl/client.h>

//デバイスクラス
class Device
{
private:
	//コンストラクタ　デストラクタ
	Device() = default;
	~Device() = default;

	Microsoft::WRL::ComPtr<ID3D12Device> device{}; //デバイスインターフェース

public:
	//インスタンス　取得　参照
	static Device& Ins() {
		static Device device;
		return device;
	}

	//デバイスの作成
	bool Create();

	//デバイスインターフェースの取得
	ID3D12Device* Get() const { return device.Get(); }
};