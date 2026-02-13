#include "Fence.h"
#include <cassert>

#include "Device.h"

//フェンスの生成
bool Fence::Create()
{
	if (Device::Ins().Get()->CreateFence(
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
	if (!event)
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
	commandQueue->Signal(fence.Get(), nextValue);
	value[idx] = nextValue++;
}