#pragma once
#include <cassert>

#include <dxgi1_6.h>
#include <utility>
#include <d3d12.h>
#include <windef.h>

//スワップチェーンクラス
class SwapChain
{
private:
	IDXGISwapChain4* swapChain; //スワップチェーンインターフェース
	DXGI_SWAP_CHAIN_DESC1 desc; //スワップチェーン記述子

public:
	SwapChain() = default; //コンストラクタ
	~SwapChain();         //デストラクタ

	//スワップチェーンの作成
	bool Create(std::pair<UINT,UINT>, IDXGIFactory6*, ID3D12CommandQueue*, HWND);

	//スワップチェーンインターフェースの取得
	IDXGISwapChain4* GetChain() const { return swapChain; }

	//スワップチェーン記述子の取得
	DXGI_SWAP_CHAIN_DESC1 GetDesc() const { return desc; }
};

