#include "Window.h"
#include "Factory.h"
#include "Device.h"
#include "ComGroup.h"
#include "SwapChain.h"
#include "Heap.h"
#include "RendTarget.h"
#include "Fence.h"
#include "SquarePoly.h"
#include "ShaderNo00.h"

class Operations
{
private:
	std::pair<UINT, UINT> size = { 1280, 960 };	//ウィンドウサイズ
	Window win;			//ウィンドウクラス
	Factory factory;	//ファクトリークラス
	Device device;		//デバイスクラス
	ComGroup command;   //コマンドグループクラス
	SwapChain swap;		//スワップチェーンクラス
	Heap descripHeapRTV;//ディスクリプタヒープクラス
	RendTarget render;	//レンダ―ターゲットクラス
	Fence fence;		//フェンスクラス
	SquarePoly square;	//四角形ポリゴンクラス
	ShaderNo00 shader00;//シェーダー00クラス

public:
	Operations() = default;	//コンストラクタ
	~Operations() = default;//デストラクタ

	//初期化
	bool Initialize(HINSTANCE instance)
	{
		//ウィンドウの生成
		if(win.Create(instance, size, "DirectX12"))			return false;

		//ファクトリーの作成
		if (factory.Create())								return false;

		//デバイスの作成
		if (device.Create())					return false;

		//コマンドアロケータ　コマンドリスト　コマンドキュー　作成
		if (command.Create(device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT)) return false;

		//スワップチェーンの作成
		if (swap.Create(size, factory.Get(), command.GetQueue(), win.GetHWND())) return false;

		//ディスクリプタヒープの作成
		if (descripHeapRTV.Create(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swap.GetDesc().BufferCount)) return false;

		//レンダ―ターゲットの作成
		if (render.Create(device.Get(), swap.GetDesc(), swap.GetChain(), descripHeapRTV.GetHeap()))	return false;

		//フェンスの作成
		if (fence.Create(device.Get()))	return false;

		//四角形の作成
		if (square.Create(device.Get()))	return false;

		//シェーダー00の作成
		if (shader00.Create())		return false;

		//No abnormalitys
		return true;
	}

	//実行
	void Execute()
	{
		MSG msg{};
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			//未使用のバッファーインデックスを取得
			const UINT backIdx = swap.GetChain()->GetCurrentBackBufferIndex();

			//フェンスの確認
			fence.Check(backIdx);

			//コマンドアロケーター　コマンドリスト　のリセット
			command.Resets(backIdx);

			//ターゲット　プレゼント → レンダーターゲット
			command.ResourceBarrier(render.Get(backIdx), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//レンダ―ターゲットの設定
			D3D12_CPU_DESCRIPTOR_HANDLE hendles[] = {render.GetHendle(device.Get(), descripHeapRTV.GetHeap(), backIdx)};
			//設定の適応
			command.GetList()->OMSetRenderTargets(1, hendles, true, nullptr);

			//レンダ―ターゲットをクリア
			float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			command.GetList()->ClearRenderTargetView(hendles[0], clearColor, 0, nullptr);

			//コマンドリストの閉鎖
			command.GetList()->Close();

			//コマンドキューにリストをセットし実行
			ID3D12CommandList* commandLists[] = { command.GetList() };
			command.GetQueue()->ExecuteCommandLists(1, commandLists);

			swap.GetChain()->Present(1, 0);

			//フェンスの更新
			fence.Updata(command.GetQueue(), backIdx);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

};

//エントリーポイント
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Operations ops;					//オペレーションクラスのインスタンス化
	if (ops.Initialize(hInstance))	//初期化　成功：異常終了
		ops.Execute();				//実行	ループ開始	
	return 0;
}