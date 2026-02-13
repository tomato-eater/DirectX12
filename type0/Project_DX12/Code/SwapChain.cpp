#include "SwapChain.h"
#include <cassert>

#include "DXGI.h"
#include "ComGroup.h"
#include "Window.h"

//スワップチェーンの作成
bool SwapChain::Create(ID3D12CommandQueue* queue)
{
	const auto size = Window::Ins().Size();
	//スワップチェーン記述子の設定
	desc.BufferCount = 2;								//バッファ数(ダブルバッファ)
	desc.Width = size.first;							//幅
	desc.Height = size.second;							//高さ
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			//色フォーマット
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //レンダーターゲットとして使用
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//フリップモデル
	desc.SampleDesc.Count = 1;							//マルチサンプリングしない
	desc.SampleDesc.Quality = 0;						//クオリティレベル 0
	desc.Stereo = FALSE;								//ステレオ表示かどうか
	desc.Scaling = DXGI_SCALING_STRETCH;				//ウィンドウサイズに合わせて伸縮
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;		//アルファモード指定なし
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;	//ウィンドウモード　フルスクリーンモード　切替可能

	//スワップチェーンの作成
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1{};
	if (DXGI::Ins().Factory()->CreateSwapChainForHwnd(
		queue,						//コマンドキュー
		Window::Ins().Get(),	//ウィンドウハンドル
		&desc,					    //スワップチェーン記述子
		nullptr,					//フルスクリーン設定
		nullptr,					//制限出力
		&swapChain1					//スワップチェーンインターフェース1
	) != S_OK){
		assert(false && "スワップチェーンの作成ー失敗ー");
		return true;
	}
	//スワップチェーンインターフェースの取得
	if (swapChain1->QueryInterface(
		IID_PPV_ARGS(&swapChain) //スワップチェーンインターフェース4
	) != S_OK){
		assert(false && "スワップチェーンインターフェースの取得ー失敗ー");
		return true;
	}
	return false;
}