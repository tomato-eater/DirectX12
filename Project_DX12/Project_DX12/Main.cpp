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
#include "DeltaPoly.h"
#include "SquarePoly.h"
#include "RootSig.h"
#include "Shader.h"
#include "PipLine.h"
#include "Camera.h"
#include "ConBuffer.h"
#include "Obj.h"
#include "DepBuffer.h"

#include "Amo.h"

//アプリケーションオペレーション
class Operations
{
private:
	pair<int, int> size = { 800,600 };
	string name = "移動 =>WASD-QE 発射 =>B 終了 =>Z";

	Window wind{};		//ウィンドウ
	Fac_Ada f_a{};		//ファクトリー_アダプター
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

	Camera camera{};	//カメラ本体
	ConBuffer camBuff{};//カメラバッファ

	DeltaPoly delPol{};	//三角形のポリゴン
	ConBuffer delBuff{};//三角形のバッファ
	Obj delObj{};		//三角形の座標_色

	SquarePoly sqrPol{};//四角形のポリゴン
	ConBuffer sqrBuff{};//四角形のバッファ
	Obj sqrObj{};		//四角形の座標_色

	Heap depsHeap{};	//デプスクリプタヒープ
	DepBuffer depBuff{};//デプスクリプタバッファ

	Amo amo{};			//弾のポリゴン_バッファ_座標_色

public:
	Operations() = default;
	~Operations() = default;

	//アプリケーションの初期化
	bool Initialize(HINSTANCE instance)
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

		//コマンドアロケータ(配列)の生成
		for (int i = 0; i < swapC.GetDesc().BufferCount; i++)
			if (comAll[i].Create(devi, D3D12_COMMAND_LIST_TYPE_DIRECT))				return false;

		//コマンドリストの生成
		if (comLis.Create(devi, comAll[0]))			return false;

		//フェンスの生成
		if (fence.Create(devi))			return false;

		//ルートシグネイチャーの生成
		if (rootSig.Create(devi))			return false;

		//シェーダーの生成
		if (shader.Create(devi))			return false;

		//パイプラインの生成
		if (pipLine.Create(devi, rootSig, shader))			return false;

		//カメラの設置
		camera.Initialize(size.first, size.second);

		//コンスタンスヒープの生成
		if (consHeap.Create(devi, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, swapC.GetDesc().BufferCount, true))			return false;
		
		//コンスタントバッファの生成
		if (camBuff.Create(devi, consHeap, sizeof(Camera::ConstBufferData), 0))			return false;


		//三角形ポリゴンの生成
		if (delPol.Create(devi))			return false;
		
		//三角形のバッファの生成
		if (delBuff.Create(devi, consHeap, sizeof(DeltaPoly::ConstBufferData), 1))			return false;
		
		//三角形の位置と色の設定
		delObj.Initialize({  0.0f,  0.5f, 5.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		//四角形ポリゴンの生成
		if (sqrPol.Create(devi))			return false;
		//四角形のバッファの生成
		if (sqrBuff.Create(devi, consHeap, sizeof(SquarePoly::ConstBufferData), 2))			return false;
		//四角形の位置と色の設定
		sqrObj.Initialize({  0.0f,  0.0f,  0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });

		//デプスクリプタヒープの生成
		if (depsHeap.Create(devi, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1))			return false;
		//デプスクリプタバッファの生成
		if (depBuff.Create(devi, depsHeap, size.first, size.second))			return false;

		return true;	//なんの問題も無かった
	}

	//アプリケーションの描画等
	void Loop()
	{
		bool fire = false;

		MSG msg{};
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			//未使用バッファのナンバーを取得
			UINT idx = swapC.GetChain()->GetCurrentBackBufferIndex();

			//フェンスの確認
			fence.Check(idx);

			//コマンドアロケータのリセット
			comAll[idx].Reset();
			//コマンドリストのリセット
			comLis.Reset(comAll[idx]);

			//コマンドリストのターゲットを変える
			comLis.Chenge(render, idx, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//コマンドリストのターゲットの設定
			D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { render.GetHandle(devi, descHeap, idx) };
			D3D12_CPU_DESCRIPTOR_HANDLE depthHand = depBuff.GetHandle();
			comLis.GetList()->OMSetRenderTargets(1, handles, false, &depthHand);
			
			//背景色の指定
			float backColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
			//レンダーターゲットのクリア
			comLis.GetList()->ClearRenderTargetView(handles[0], backColor, 0, nullptr);
			//デプスバッファのクリア
			comLis.GetList()->ClearDepthStencilView(depthHand, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
			//ルートシグネイチャーの設定
			comLis.GetList()->SetGraphicsRootSignature(rootSig.GetSign());
			//ビューポートとシザーの設定
			comLis.SetVS(size.first, size.second);

			//コンスタントバッファ用のディスクリプタヒープの設定
			ID3D12DescriptorHeap* ppHeaps[] = { consHeap.GetHeap() };
			comLis.GetList()->SetDescriptorHeaps(1, ppHeaps);
			
			// カメラ
			{	

				//カメラの設置
				camera.Updata();
			}

			comLis.GetList()->SetPipelineState(pipLine.GetPip());

			{	// モデル△
				DeltaPoly::ConstBufferData delData
				{
					DirectX::XMMatrixTranspose(delObj.GetWorld()),
					delObj.GetColor()
				};
				UINT8* pDelData{};
				delBuff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pDelData));
				memcpy(pDelData, &delData, sizeof(delData));
				delBuff.GetBuf()->Unmap(0, nullptr);
				comLis.GetList()->SetGraphicsRootDescriptorTable(1, delBuff.GetHand());

				delPol.Draw(comLis);
			}
			{	// モデル□
				SquarePoly::ConstBufferData sqrData
				{
					DirectX::XMMatrixTranspose(sqrObj.GetWorld()),
					sqrObj.GetColor()
				};
				UINT8* pSqrData{};
				sqrBuff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pSqrData));
				memcpy(pSqrData, &sqrData, sizeof(sqrData));
				sqrBuff.GetBuf()->Unmap(0, nullptr);
				comLis.GetList()->SetGraphicsRootDescriptorTable(1, sqrBuff.GetHand());

				sqrPol.Draw(comLis);
			}
			if (GetAsyncKeyState('B') && !fire)
			{	// 弾発射
				amo.Summon(devi, consHeap, sqrObj, comLis);
				fire = true;
			}
			if (!GetAsyncKeyState('B') && fire) fire = false;


			amo.Updata(comLis);

			//弾丸の位置を見る
			amo.Check(delObj);

			sqrObj.Updata();

			comLis.Chenge(render, idx, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			comLis.GetList()->Close();

			ID3D12CommandList* ppComLiss[] = { comLis.GetList() };
			comQ.GetQ()->ExecuteCommandLists(_countof(ppComLiss), ppComLiss);


			swapC.GetChain()->Present(1, 0);
			fence.SetNext(comQ, idx);

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (GetAsyncKeyState('Z')) break;
		}
	}

};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int nCmdShow)
{
	Operations op;

	if (op.Initialize(instance)) op.Loop();
	
	return 0;
}