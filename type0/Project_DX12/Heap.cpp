#include "Heap.h"

//デストラクタ
Heap::~Heap()
{
	if (heap)
	{
		heap->Release();
		heap = nullptr;
	}
}

//ヒープの作成
bool Heap::Create(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE getType, UINT num, bool shaderVisible)
{
	//ヒープ記述子の設定
	D3D12_DESCRIPTOR_HEAP_DESC desc{};//ヒープ記述子
	desc.Type = getType;			  //ヒープタイプ
	desc.NumDescriptors = num;		  //デスクリプタ数
	desc.NodeMask = 0;				  //ノードマスク(アダプター1つ)
	desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
									  //↑ ヒープフラグ
	//ヒープの作成
	if (device->CreateDescriptorHeap(
		&desc,							//ヒープ記述子
		IID_PPV_ARGS(&heap)				//ヒープインターフェース
	) != S_OK)
	{
		assert(false && "ヒープの作成ー失敗ー");
		return true;
	}

	return false;
}