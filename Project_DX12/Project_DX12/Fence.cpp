#include "Fence.h"

Fence::~Fence()
{
	if (fence)
	{
		fence->Release();
		fence = nullptr;
	}
}

bool Fence::Create(Device& devi)
{
	auto hr = devi.GetDev()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(hr))
	{
		assert(false && "フェンス作成ー失敗ー");
		return true;
	}

	event = CreateEvent(nullptr, false, false, "Wait_GPU");
	if (!event)
	{
		assert(false && "イベント作成ー失敗ー");
		return true;
	}

	return false;
}

void Fence::Check(UINT idx)
{
	if (fence->GetCompletedValue() < value[idx])
	{
		fence->SetEventOnCompletion(value[idx], event);
		WaitForSingleObject(event, INFINITE);
	}
}

void Fence::SetNext(ComQue& comQue, UINT idx)
{
	comQue.GetQ()->Signal(fence, nextV);
	value[idx] = nextV;
	nextV++;
}