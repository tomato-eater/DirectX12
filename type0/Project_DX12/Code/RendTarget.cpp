#include "RendTarget.h"

// デストラクタ
RendTarget::~RendTarget()
{
	for (auto target : renderTargets)
	{
		if (target)
		{
			target->Release();
			target = nullptr;
		}
	}
}

// レンダーターゲットの作成
bool RendTarget::Create(ID3D12Device* device, DXGI_SWAP_CHAIN_DESC1 desc, IDXGISwapChain4* chain, ID3D12DescriptorHeap* heap)
{
	// レンダーターゲットの数を取得
	UINT bufferCount = desc.BufferCount;

	// レンダーターゲット分の領域を確保
	renderTargets.resize(bufferCount);

	// ヒープタイプの確認
	auto heapType = heap->GetDesc().Type;
	if (heapType != D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	{
		assert(false && "RTVヒープタイプではありません");
		return true;
	}

	//ディスクリプタヒープのハンドルを取得
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();

	//バックバッファの生成
	for (UINT i = 0; i < bufferCount; i++)
	{
		// スワップチェーンからバックバッファを取得
		if (chain->GetBuffer(
			i,										// バックバッファのインデックス
			IID_PPV_ARGS(&renderTargets[i])			// バックバッファ
		) != S_OK)
		{
			assert(false && "バックバッファの取得ー失敗ー");
			return true;
		}

		device->CreateRenderTargetView(renderTargets[i], nullptr, handle);
		handle.ptr += device->GetDescriptorHandleIncrementSize(heapType);
	}

	return false;
}

//ディスクリプタハンドルを取得
D3D12_CPU_DESCRIPTOR_HANDLE RendTarget::GetHendle(ID3D12Device* device, ID3D12DescriptorHeap* heap, UINT idx)
{
	if (idx >= renderTargets.size() || !renderTargets[idx])
		assert(false && "レンダ―ターゲットの範囲外");

	//ディスクリプタヒープのハンドルを取得
	auto hendle = heap->GetCPUDescriptorHandleForHeapStart();
	
	//ディスクリプタのサイズを取得
	auto rtvDiscriptSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	//ハンドルを微調整
	hendle.ptr += idx * rtvDiscriptSize;
	return hendle;
}