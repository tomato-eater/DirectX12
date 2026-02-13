#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>

//スワップチェーンクラス
class SwapChain
{
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain{};//スワップチェーンインターフェース
	DXGI_SWAP_CHAIN_DESC1 desc{};						//スワップチェーンデスク

public:
	//コンストラクタ　デストラクタ
	SwapChain() = default;
	~SwapChain() = default;

	//スワップチェーンの作成
	bool Create(ID3D12CommandQueue*);

	//スワップチェーンインターフェースの取得
	IDXGISwapChain4* Chain() const { return swapChain.Get(); }

	//スワップチェーン記述子の取得
	const DXGI_SWAP_CHAIN_DESC1& Desc() const { return desc; }
};

