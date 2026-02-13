#include "TextureBuff.h"
using namespace DirectX;

constexpr float destTargetToView = -5.0f;

TextureBuff::~TextureBuff()
{
	if (constBuff)
	{
		constBuff->Release();
		constBuff = nullptr;
	}
}

bool TextureBuff::Create(std::pair<float, float> size, ID3D12Device* device, ID3D12DescriptorHeap* heap)
{
	//3D空間への準備
	{
		eye = XMFLOAT3(0.0f, 0.0f, destTargetToView);
		target = XMFLOAT3(0.0f, 0.0f, 0.0f);
		up = XMFLOAT3(0.0f, 1.0f, 0.0f);

		XMMATRIX look = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV2, static_cast<float>(size.first) / static_cast<float>(size.second), 1.0f, 10.0f);

		matrix *= worldMat;
		matrix *= look;
		matrix *= proj;
	}

	auto memorySize = (static_cast<UINT64>(sizeof(matrix) + 0xff) & ~0xff);

	//バッファの作成
	{
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProp.CreationNodeMask = 1;
		heapProp.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC resouceDesc{};
		resouceDesc.Dimension		 = D3D12_RESOURCE_DIMENSION_BUFFER;
		resouceDesc.Alignment		 = 0;
		resouceDesc.Width			 = memorySize;
		resouceDesc.Height			 = 1;
		resouceDesc.DepthOrArraySize = 1;
		resouceDesc.MipLevels		 = 1;
		resouceDesc.Format			 = DXGI_FORMAT_UNKNOWN;
		resouceDesc.SampleDesc		 = { 1,0 };
		resouceDesc.Layout			 = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resouceDesc.Flags			 = D3D12_RESOURCE_FLAG_NONE;

		device->CreateCommittedResource(
			&heapProp,							//上記のヒーププロパティ
			D3D12_HEAP_FLAG_NONE,				//ヒープフラッグ
			&resouceDesc,						//上記のリソースデスク
			D3D12_RESOURCE_STATE_GENERIC_READ,	//リソースの状態
			nullptr,							//展開先
			IID_PPV_ARGS(&constBuff));			//ぶち込む
		if (!constBuff) {
			assert(false && "テクスチャのコンスタントバッファの作成ー失敗ー");
			return true;
		}
	}

	XMMATRIX* pData{};
	constBuff->Map(0, nullptr, (void**)&pData);
	*pData = matrix;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = memorySize;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	device->CreateConstantBufferView(&cbvDesc, handle);

	return false;
}