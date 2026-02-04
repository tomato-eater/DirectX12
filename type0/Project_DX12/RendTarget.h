#pragma once
#include <cassert>

#include <d3d12.h>
#include <vector>

#include <dxgi1_6.h>

class RendTarget
{
private:
	std::vector<ID3D12Resource*> renderTargets; //レンダーターゲット

public:
	RendTarget() = default;	//コンストラクタ
	~RendTarget();			//デストラクタ

	//レンダーターゲットの作成
	bool Create(ID3D12Device*, DXGI_SWAP_CHAIN_DESC1,IDXGISwapChain4*, ID3D12DescriptorHeap*);

	//レンダーターゲットの取得
	ID3D12Resource* Get(UINT index) const { return renderTargets[index]; }

	//ディスクリプタハンドルの取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetHendle(ID3D12Device*,ID3D12DescriptorHeap*, UINT);
};

