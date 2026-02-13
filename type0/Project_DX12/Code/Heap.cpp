#include "Heap.h"
#include <cassert>

#include "Device.h"

//ヒープの作成
bool HeapReader::Heap::Set(D3D12_DESCRIPTOR_HEAP_TYPE getType, UINT num, bool shaderVisible)
{
	//ヒープ記述子の設定
	D3D12_DESCRIPTOR_HEAP_DESC desc{};//ヒープ記述子
	desc.Type = getType;			  //ヒープタイプ
	desc.NumDescriptors = num;		  //デスクリプタ数
	desc.NodeMask = 0;				  //ノードマスク(アダプター1つ)
	desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//↑ ヒープフラグ
	//ヒープの作成
	if (Device::Ins().Get()->CreateDescriptorHeap(
		&desc,						  //ヒープ記述子
		IID_PPV_ARGS(&heap)			  //ヒープインターフェース
	) != S_OK)
	{
		assert(false && "ヒープの作成ー失敗ー");
		return false;
	}

	free.resize(num, true);

	return true;
}

//ナンバーの取得
int  HeapReader::Heap::GetNum()
{
	for (int i = 0; i < free.size(); i++)
		if (free.at(i)) {
			free.at(i) = false;
			return i;
		}
	return -1;
}

//ヒープリーダーでのヒープの確認　作成
bool HeapReader::Create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT no, bool sVisible)
{
	//作成済か否か
	if (map.find(type) != map.end())
		return true;
	//作成
	auto heap = std::make_unique<Heap>();
	if (heap->Set(type, no, sVisible)) {
		map.emplace(type, std::move(heap));
		return false;
	}
	return true;
}

//ヒープリーダーでのヒープの取得
ID3D12DescriptorHeap* HeapReader::GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE type) const
{
	const auto get = map.find(type);
	if (get == map.end())
		return nullptr;
	return get->second->GetHeap();
}

//ヒープナンバーの取得
int HeapReader::GetNum(D3D12_DESCRIPTOR_HEAP_TYPE type) const
{
	const auto  get = map.find(type);
	if (get == map.end())
		return -1;
	return get->second->GetNum();
}