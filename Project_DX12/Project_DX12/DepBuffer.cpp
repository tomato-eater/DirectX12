#include "DepBuffer.h"

DepBuffer::~DepBuffer()
{
	if (depthBuffer)
	{
		depthBuffer->Release();
		depthBuffer = nullptr;
	}
}

bool DepBuffer::Create(Device& devi, Heap& heap, int wi, int hi)
{
	D3D12_HEAP_PROPERTIES hProp{};
	hProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC rDesc{};
	rDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rDesc.Width = wi;
	rDesc.Height = hi;
	rDesc.DepthOrArraySize = 1;
	rDesc.MipLevels = 1;
	rDesc.Format = DXGI_FORMAT_D32_FLOAT;
	rDesc.SampleDesc = { 1, 0 };
	rDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	rDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = rDesc.Format;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	auto hr = devi.GetDev()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&depthBuffer));
	if (FAILED(hr))
	{
		assert(false && "デプスバッファ作成ー失敗ー");
		return true;
	}
	auto hType = heap.GetType();
	if (hType != D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
	{
		assert(false && "ディスクリプタヒープタイプが違う");
		return true;
	}
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = rDesc.Format;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	handle = heap.GetHeap()->GetCPUDescriptorHandleForHeapStart();
	
	devi.GetDev()->CreateDepthStencilView(depthBuffer, &dsvDesc, handle);

	return false;
}