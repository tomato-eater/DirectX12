#include "ComLis.h"

ComLis::~ComLis()
{
	if (commandList)
	{
		commandList->Release();
		commandList = nullptr;
	}
}

bool ComLis::Create(Device& devi, ComAll& comAll)
{
	auto hr = devi.GetDev()->CreateCommandList(0, comAll.GetType(), comAll.GetAll(), nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(hr))
	{
		assert(false && "コマンドリスト作成―失敗ー");
		return true;
	}
	commandList->Close();
	return false;
}

void ComLis::Chenge(Target& tar, UINT idx, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = tar.GetTarget(idx);
	barrier.Transition.StateBefore = from;
	barrier.Transition.StateAfter = to;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(1, &barrier);
}

void ComLis::SetVS(int wi, int hi)
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
	rect.right = wi;
	rect.bottom = hi;
	commandList->RSSetScissorRects(1, &rect);
}