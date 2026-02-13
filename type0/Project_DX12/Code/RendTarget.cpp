#include "RendTarget.h"
#include <cassert>

#include "Heap.h"
#include "Device.h"


// レンダーターゲットの作成
bool RendTarget::Create(const DXGI_SWAP_CHAIN_DESC1& desc, IDXGISwapChain4* chain)
{
	// レンダーターゲット分の領域を確保
	renderTargets.resize(desc.BufferCount);

	//バックバッファの生成
	for (UINT i = 0; i < renderTargets.size(); i++)	{
		// スワップチェーンからバックバッファを取得
		if (chain->GetBuffer(
			i,								// バックバッファのインデックス
			IID_PPV_ARGS(&renderTargets.at(i))	// バックバッファ
		) != S_OK)
		{
			assert(false && "バックバッファの取得ー失敗ー");
			return true;
		}
	}

	//ビューの作成
	if (CreateRender())	return true;
	//if (CreateShader())	return true;

	return false;
}

//レンダービューの作成
bool RendTarget::CreateRender()
{
	//タイプの指定
	auto type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//ヒープの取得
	auto heap = HeapReader::Ins().GetHeap(type);
	//ハンドルのサイズ取得
	const auto size = Device::Ins().Get()->GetDescriptorHandleIncrementSize(type);
	for (UINT i = 0; i < renderTargets.size(); i++) {
		//ヒープのデスク数を取得
		const auto idx = HeapReader::Ins().GetNum(type);
		if (idx < 0) {
			assert(false && "ヒープデスクの取得ー失敗ー");
			return true;
		}
		//ハンドルの取得
		auto handle = heap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += idx * size;
		//紐づける
		Device::Ins().Get()->CreateRenderTargetView(renderTargets.at(i).Get(), nullptr, handle);
	}
	return false;
}

//シェーダービューの作成
bool RendTarget::CreateShader()
{
	//タイプの指定
	auto type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//ヒープの取得
	auto heap = HeapReader::Ins().GetHeap(type);
	//ハンドルのサイズ取得
	const auto size = Device::Ins().Get()->GetDescriptorHandleIncrementSize(type);
	for (UINT i = 0; i < renderTargets.size(); i++)	{
		//ヒープのデスク数を取得
		const auto idx = HeapReader::Ins().GetNum(type);
		if (idx < 0) {
			assert(false && "ヒープデスクの取得ー失敗ー");
			return true;
		}
		//シェーダーリソースビューの設定
		D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		desc.Format = renderTargets.at(i)->GetDesc().Format;
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = renderTargets.at(i)->GetDesc().MipLevels;
		//ハンドルの取得
		auto handle = heap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += idx * size;
		//紐づける
		Device::Ins().Get()->CreateShaderResourceView(renderTargets.at(i).Get(), &desc, handle);
	}
	return false;
}

//レンダ―ターゲットのサイズ取得
std::pair<float, float> RendTarget::Size() const
{
	if (renderTargets.empty()) {
		assert(false && "レンダ―ターゲットエンプティ");
		return{};
	}
	const auto desc = renderTargets.at(0)->GetDesc();
	return{ static_cast<float>(desc.Width), static_cast<float>(desc.Height) };
}

//ディスクリプタハンドルRTVを取得
D3D12_CPU_DESCRIPTOR_HANDLE RendTarget::RTVhandle(UINT idx) const
{
	if (idx >= renderTargets.size() || idx < 0)
		assert(false && "レンダ―ターゲットの範囲外");

	//ヒープタイプの指定
	auto type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	//ヒープのハンドルの取得
	auto handle = HeapReader::Ins().GetHeap(type)->GetCPUDescriptorHandleForHeapStart();
	
	//ディスクリプタのサイズを取得
	auto rtvDiscriptSize = Device::Ins().Get()->GetDescriptorHandleIncrementSize(type);

	//ハンドルを微調整
	handle.ptr += idx * rtvDiscriptSize;;
	return handle;
}