#include "Fence.h"

//デストラクタ
Fence::~Fence()
{
	if (fence)
	{
		fence->Release();
		fence = nullptr;
	}
}

//フェンスの生成
bool Fence::Create(ID3D12Device* device)
{
	if (device->CreateFence(
		0,						//初期値
		D3D12_FENCE_FLAG_NONE,	//フラッグ
		IID_PPV_ARGS(&fence)	//フェンス
	))
	{
		assert(false && "フェンスの作成ー失敗ー");
		return true;
	}
	event = CreateEvent(
		nullptr,		//LPセキュリティー属性
		false,			//マニュアルリセット
		false,			//初期状態
		"Wait_GPU"		//イベントの名前
	);
	if (!fence)
	{
		assert(false && "イベントの作成ー失敗ー");
		return true;
	}

	return false;
}

//フェンスの確認
void Fence::Check(UINT idx)
{
	//フェンス値が指定された値まで達しているか
	if (fence->GetCompletedValue() < value[idx])
	{
		//GPUがフェンス値に達するまで待機
		fence->SetEventOnCompletion(value[idx], event);
		WaitForSingleObject(event, INFINITE);
	}
}

//フェンス値の更新
void Fence::Updata(ID3D12CommandQueue* commandQueue, UINT idx)
{
	commandQueue->Signal(fence, nextValue);
	value[idx] = nextValue++;
}