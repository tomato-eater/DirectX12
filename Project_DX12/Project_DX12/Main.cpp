#include "Window.h"
#include "Fac_Ada.h"
#include "Device.h"
#include "ComQue.h"
#include "SwapC.h"
#include "Heap.h"
#include "Target.h"
#include "ComAll.h"
#include "ComLis.h"
#include "Fence.h"
#include "RootSig.h"
#include "Shader.h"
#include "PipLine.h"
#include "DepBuffer.h"

#include "CameraPre.h"
#include "Delta.h"
#include "Square.h"
//#include "Amo.h"
#include "CubePre.h"

#include "PosPro.h"
#include "ScRootSig.h"
#include "ScShader.h"
#include "ScPipLine.h"

class Operations
{
private:
	pair<int, int> size = { 1280,960 };
	string name = "移動 =>WASD-QE 発射 =>B 終了 =>Z";

	Window wind{};		//ウィンドウ
	Fac_Ada f_a{};		//ファクトリー　アダプター
	Device devi{};		//デバイス
	ComQue comQ{};		//コマンドキュー
	SwapC swapC{};		//スワップチェイン
	Heap descHeap{};	//ディスクリプタヒープ
	Target render{};	//レンダーターゲット
	ComAll comAll[2]{};	//コマンドアロケーター
	ComLis comLis{};	//コマンドリスト

	Fence fence{};		//フェンス

	RootSig rootSig{};	//ルートシグネイチャー
	Shader shader{};	//シェーダー
	PipLine pipLine{};	//パイプライン
	Heap consHeap{};	//コンスタンスヒープ

	Heap depsHeap{};	//デプスクリプタヒープ
	DepBuffer depBuff{};//デプスクリプタバッファ

	CameraPre camera{};	//カメラ
	Delta delta{};		//三角形　的
	Square sqare{};		//四角形　自身
	//Amo amo{};			//四角形　弾

	CubePre cube{};		//立方体　弾

	PosPro posPro{};	//フィルター
	ScRootSig scRootSig{};	//スクリーンルートシグネイチャー
	ScShader scShader{};	//スクリーンシェーダー
	ScPipLine scPipLine{};	//スクリーンパイプライン

public:
	Operations() = default;
	~Operations() = default;

	bool Initialize(HINSTANCE instance)	//DirectXの準備
	{
		//ウィンドウの生成
		if (wind.Create(instance, name, size.first, size.second))			return false;

		//ファクトリー_アダプターの生成
		if (f_a.Create())			return false;

		//デバイスの生成
		if (devi.Create(f_a))			return false;

		//コマンドキューの生成
		if (comQ.Create(devi))			return false;

		//スワップチェインの生成
		if (swapC.Create(wind, f_a, comQ, size.first, size.second))			return false;

		//ディスクリプタヒープの生成
		if (descHeap.Create(devi, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapC.GetDesc().BufferCount))			return false;

		//レンダーターゲットの生成
		if (render.Create(devi, swapC, descHeap))			return false;

		//コマンドアロケータの生成　配列分
		for (int i = 0; i < swapC.GetDesc().BufferCount; i++)
			if (comAll[i].Create(devi, D3D12_COMMAND_LIST_TYPE_DIRECT))				return false;

		//コマンドリストの生成
		if (comLis.Create(devi, comAll[0]))			return false;

		//フェンスの生成
		if (fence.Create(devi))			return false;

		//ルートシグネイチャーの生成
		if (rootSig.Create(devi))			return false;

		//シェーダーの生成
		if (shader.Create())			return false;

		//パイプラインの生成
		if (pipLine.Create(devi, rootSig, shader))			return false;

		//コンスタンスヒープの生成
		if (consHeap.Create(devi, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, swapC.GetDesc().BufferCount, true))			return false;

		//デプスクリプタヒープの生成
		if (depsHeap.Create(devi, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1))		return false;

		//デプスバッファの生成
		if (depBuff.Create(devi, depsHeap, size.first, size.second))		return false;

		//フィールター
		if (posPro.Create(render, devi, descHeap)) return false;

		//スクリーンルートシグネイチャーの生成
		if (scRootSig.Create(devi))	return false;

		//スクリーンシェーダーの生成
		if (scShader.Create())		return false;

		//スクリーンパイプラインの生成
		if (scPipLine.Create(devi, scRootSig, scShader))	return false;

		//特に問題なし！
		return true;
	}

	void Loop()
	{
		bool fire = false;	//射撃済みかの確認用

		if (camera.Create(size.first, size.second, devi, consHeap, 0)) return;	//カメラの生成
		
		if (delta.Create(devi, consHeap, 1))			return;			//的の生成
		delta.Set({ 0.0f,  0.5f, 5.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });	//的の初期位置、初期色

		if (sqare.Create(devi, consHeap, 2))			return;			//自身の生成
		sqare.Set({ 0.0f,  0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });	//自身の初期値、初期色

		MSG msg{};
		while (GetMessage(&msg, nullptr, 0, 0))		//ループ開始
		{
			//未使用のバッファーを確認
			UINT idx = swapC.GetChain()->GetCurrentBackBufferIndex();	

			//フェンスの確認
			fence.Check(idx);

			//的と弾丸の距離を確認
			//amo.Check(delta.GetObj());
			cube.Check(delta.GetObj());

			//コマンドアロケーター_コマンドリスト をリセット
			comAll[idx].Reset();
			comLis.Reset(comAll[idx]);

			//コマンドリストのターゲットを変更
			comLis.Chenge(render, idx, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//ターゲットの設定
			D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { render.GetHandle(devi, descHeap, idx) };
			D3D12_CPU_DESCRIPTOR_HANDLE depthHand = depBuff.GetHandle();
			comLis.GetList()->OMSetRenderTargets(1, handles, false, &depthHand);
			
			//背景色の設定
			float backColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
			comLis.GetList()->ClearRenderTargetView(handles[0], backColor, 0, nullptr);
			comLis.GetList()->ClearDepthStencilView(depthHand, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
			comLis.GetList()->SetGraphicsRootSignature(rootSig.GetSign());
			comLis.SetVS(size.first, size.second);

			ID3D12DescriptorHeap* ppHeaps[] = { consHeap.GetHeap() };
			comLis.GetList()->SetDescriptorHeaps(1, ppHeaps);

			comLis.GetList()->SetPipelineState(pipLine.GetPip());

			camera.Change(comLis);	//カメラの位置の変更受付
			camera.Update();		//カメラの更新

			delta.Update(comLis);	//的の更新
			sqare.Update(comLis);	//自身の更新

			//amo.Update(comLis);	//弾の更新
			cube.Update(comLis);	//弾(立体)の更新

			if (GetAsyncKeyState('B') && !fire)					//弾発射
			{
				//amo.Summon(devi, consHeap, sqare.GetObj(), comLis);  //2D
				cube.Summon(devi, consHeap, sqare.GetObj(), comLis);	//3D
				fire = true;
			}
			if (!GetAsyncKeyState('B') && fire) fire = false;	//発射後


			{//フィルター処理へ
				comLis.ScChenge(posPro.GetResource(), idx, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

				D3D12_CPU_DESCRIPTOR_HANDLE scRtvHand[] = { posPro.GetRtvHeap()->GetCPUDescriptorHandleForHeapStart() };
				auto scDsvHand = depBuff.GetHandle();
				comLis.GetList()->OMSetRenderTargets(1, scRtvHand, false, &scDsvHand);

				float scBackColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				comLis.GetList()->ClearRenderTargetView(scRtvHand[0], scBackColor, 0, nullptr);

				auto rtvHeap = descHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart();
				rtvHeap.ptr += idx * devi.GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

				comLis.GetList()->OMSetRenderTargets(1, &rtvHeap, false, nullptr);
				comLis.GetList()->ClearRenderTargetView(rtvHeap, scBackColor, 0, nullptr);

				comLis.SetVS(size.first, size.second);

				comLis.GetList()->SetPipelineState(scPipLine.GetPip());
				comLis.GetList()->SetGraphicsRootSignature(scRootSig.GetSign());

				//ID3D12DescriptorHeap* posSrvHeap[] = { posPro.GetSrvHeap() };
				//comLis.GetList()->SetDescriptorHeaps(1, posSrvHeap);

				auto gpuHandle = posPro.GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
				comLis.GetList()->SetGraphicsRootDescriptorTable(0, gpuHandle);

				//全画面にフィルター四角形を描画
				posPro.Draw(comLis);
			}
			
			//レンダーターゲットをプレゼント用に変更
			comLis.Chenge(render, idx, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			//コマンドリストのクローズ
			comLis.GetList()->Close();

			//キューにリストをセットして実行
			ID3D12CommandList* ppComLiss[] = { comLis.GetList() };
			comQ.GetQ()->ExecuteCommandLists(_countof(ppComLiss), ppComLiss);

			//プレゼント
			swapC.GetChain()->Present(1, 0);

			//Zキーで終了
			if (GetAsyncKeyState('Z')) break;

			//次のフレーム用のフェンスセット
			fence.SetNext(comQ, idx);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int nCmdShow)
{
	Operations op;

	if (op.Initialize(instance)) op.Loop();
	
	return 0;
}