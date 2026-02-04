#pragma once
#include <cassert>

#include <d3d12.h>
#include <vector>

//ヒープクラス
class Heap
{
private:
	ID3D12DescriptorHeap* heap; //ヒープ

public:
	Heap() = default;	//コンストラクタ
	~Heap();			//デストラクタ

	//ヒープの作成
	bool Create(ID3D12Device*, D3D12_DESCRIPTOR_HEAP_TYPE, UINT, bool shaderVisible = false);

	//ヒープの取得
	ID3D12DescriptorHeap* GetHeap() const { return heap; }

};

