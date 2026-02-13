#pragma once

#include <dxgi1_6.h>
#include <wrl/client.h>

//ファクトリー_アダプタークラス
class DXGI
{
	//コンストラクタ　デストラクタ
	DXGI() = default;
	~DXGI() = default;
private:
	Microsoft::WRL::ComPtr<IDXGIFactory6> factory{};	//ファクトリーインターフェース
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter{};	//アダプターインターフェース

public:
	//インスタンス　取得　参照
	static DXGI& Ins() {
		static DXGI dxgi;
		return dxgi;
	}

	//ファクトリーインターフェースの作成
	bool CreateFactory();

	//アダプターインターフェースの作成
	bool CreateAdapter();

	//ファクトリーインターフェースの取得
	IDXGIFactory6* Factory() const { return factory.Get(); }

	//アダプターインターフェースの取得
	IDXGIAdapter1* Adapter() const { return adapter.Get(); }
};