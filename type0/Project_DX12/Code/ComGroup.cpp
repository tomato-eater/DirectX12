#include "ComGroup.h"

//デストラクタ
ComGroup::~ComGroup()
{
	if (commandList)
	{
		commandList->Release();
		commandList = nullptr;
	}
	if (commandAllocator)
	{
		commandAllocator->Release();
		commandAllocator = nullptr;
	}
}

//コマンドアロケータ コマンドリスト　コマンドキュー 作成
bool ComGroup::Create(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
{
	//コマンドアロケータの作成
	{	
		if (device->CreateCommandAllocator(
			type,									//コマンドリストのタイプ
			IID_PPV_ARGS(&commandAllocator)			//コマンドアロケータ
		) != S_OK)
		{
			assert(false && "コマンドアロケータの作成ー失敗ー");
			return true;
		}
	}
	//コマンドリストの作成
	{
		if (device->CreateCommandList(
			0,										//ノードマスク
			type,									//コマンドリストのタイプ
			commandAllocator,						//コマンドアロケータ
			nullptr,								//パイプラインステートオブジェクト
			IID_PPV_ARGS(&commandList)				//コマンドリスト
		) != S_OK)
		{
			assert(false && "コマンドリストの作成ー失敗ー");
			return true;
		}
		commandList->Close();
	}
	//コマンドキューの作成
	{
		D3D12_COMMAND_QUEUE_DESC desc{};//コマンドキュー記述子

		desc.Type = type;				//コマンドリストのタイプ
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; //優先度 (通常)
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; //フラグ(なし)
		desc.NodeMask = 0;			//ノードマスク(アダプター1つ)

		if (device->CreateCommandQueue(
			&desc,						
			IID_PPV_ARGS(&commandQueue)			//コマンドキュー
		) != S_OK)
		{
			assert(false && "コマンドキューの作成ー失敗ー");
			return true;
		}
	}
	return false;
}

//レンダ―ターゲットを各々変更
void ComGroup::ResourceBarrier(ID3D12Resource* renderTarget, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;	//タイプ
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;		//フラッグ
	barrier.Transition.pResource = renderTarget;			//リソース
	barrier.Transition.StateBefore = from;					//変更前
	barrier.Transition.StateAfter = to;						//変更後
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;//UINT型のなんか

	//変更完了
	commandList->ResourceBarrier(1, &barrier);
}

//ビューポート　シザー　の設定
void ComGroup::SetVS(float wi, float hi)
{
	D3D12_VIEWPORT view{};
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width = static_cast<float>(wi);
	view.Height = static_cast<float>(hi);
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	commandList->RSSetViewports(1, &view);

	D3D12_RECT rect{};
	rect.left = 0;
	rect.top = 0;
	rect.right = static_cast<UINT>(wi);
	rect.bottom = static_cast<UINT>(hi);
	commandList->RSSetScissorRects(1, &rect);
}

//アロケータ　リスト　のリセット
void ComGroup::Resets(UINT idx)
{
	commandAllocator->Reset();
	commandList->Reset(commandAllocator, nullptr);
}