#include "Window.h"
#include "Device.h"
#include "ComGroup.h"
#include "SwapChain.h"
#include "Heap.h"
#include "RendTarget.h"
#include "Fence.h"
#include "Shader.h"
#include "RootSig.h"
#include "PipLine.h"
#include "ObjManager.h"

class Operations
{
private:
	ComGroup command{};		//コマンド アロケータ_リスト_キュー クラス
	SwapChain swap{};		//スワップチェインクラス
	RendTarget target{};	//レンダ―ターゲットクラス
	Fence fence{};			//フェンスクラス
	RootSig root00{};	//ルートシグネチャークラス
	
	PipLine pip00{};	//パイプラインステートクラス

	Shader sha00{};//シェーダー00クラス

public:
	//コンストラクタ　デストラクタ
	Operations() = default;	
	~Operations() = default;

	//初期化
	bool Initialize(HINSTANCE instance)
	{
		//ウィンドウの生成
		if (Window::Ins().Create(instance, { 1600, 900 }, "DirectX12"))	return false;

		//デバイスの作成
		if (Device::Ins().Create())	return false;

		//コマンドアロケータ　コマンドリスト　コマンドキュー　作成
		if (command.Create(2, D3D12_COMMAND_LIST_TYPE_DIRECT))	return false;

		//スワップチェーンの作成
		if (swap.Create(command.Queue())) return false;
		
		//RTVヒープの作成
		if (HeapReader::Ins().Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swap.Desc().BufferCount, false))	return false;

		//SRVヒープの作成
		if (HeapReader::Ins().Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 32, true))	return false;
		
		//レンダ―ターゲットの作成
		if (target.Create(swap.Desc(), swap.Chain()))	return false;

		//フェンスの作成
		if (fence.Create())	return false;

		//ルートシグネチャー(スクリーン)の作成
		if (root00.Create<Root2D>())		return false;

		//シェーダー00の作成
		if (sha00.Create("Code/HLSL/Shader00.hlsl"))		return false;

		//パイプラインステートの作成
		if (pip00.Create(root00.Get(), sha00.VS(), sha00.PS(), true, Model3D::Layout()))	return false;
	
		//ゲームオブジェクトを作成
		//ObjManager::Ins().Create();
		
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
			const UINT backIdx = swap.Chain()->GetCurrentBackBufferIndex();

			//フェンスの確認
			fence.Check(backIdx);

			//コマンドアロケーター　コマンドリスト　のリセット
			command.Resets(backIdx);

			//ヒープの設定
			ID3D12DescriptorHeap* pSRV[] = { HeapReader::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)};
			command.List()->SetDescriptorHeaps(1, pSRV);

			//ターゲット　プレゼント → レンダーターゲット
			command.ResourceBarrier(target.Get(backIdx), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//レンダ―ターゲットの設定　登録
			D3D12_CPU_DESCRIPTOR_HANDLE hendles[] = {target.RTVhandle(backIdx)};
			command.List()->OMSetRenderTargets(1, hendles, true, nullptr);

			//ビューボードの設定
			const auto size = target.Size();
			command.SetVS(size.first, size.second);

			//レンダ―ターゲットをクリア
			float clearColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
			command.List()->ClearRenderTargetView(hendles[0], clearColor, 0, nullptr);

			command.List()->SetGraphicsRootSignature(root00.Get());
			command.List()->SetPipelineState(pip00.Get());

			command.List()->SetGraphicsRootDescriptorTable(0, HeapReader::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetGPUDescriptorHandleForHeapStart());
			//square.Drow(command.List());

			//ターゲット　レンダーターゲット → プレゼント
			command.ResourceBarrier(target.Get(backIdx), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

			//コマンドリストの閉鎖
			command.List()->Close();

			//コマンドキューにリストをセットし実行
			ID3D12CommandList* commandLists[] = { command.List() };
			command.Queue()->ExecuteCommandLists(1, commandLists);

			swap.Chain()->Present(1, 0);

			//フェンスの更新
			fence.Updata(command.Queue(), backIdx);

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