#pragma once

#include <d3d12.h>
#include <unordered_map>
#include <wrl/client.h>
#include <memory>
#include <vector>
	
//ヒープクラスの制御クラス
class HeapReader
{
private:
	//コンストラクタ　デストラクタ
	HeapReader() = default;
	~HeapReader() { map.clear(); }

	//ヒープクラス
	class Heap
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap{}; //ヒープ
		std::vector<bool> free{};		//ヒープナンバー

	public:
		//コンストラクタ　デストラクタ
		Heap() = default;
		~Heap() { free.clear(); }

		//ヒープの作成
		bool Set(D3D12_DESCRIPTOR_HEAP_TYPE, UINT, bool);
		
		//ヒープの取得
		ID3D12DescriptorHeap* GetHeap() const { return heap.Get(); }

		//ナンバーの取得
		int GetNum();
	};

	std::unordered_map<UINT, std::unique_ptr<Heap>> map{};//ヒープマップ

public:

	//インスタンス　取得　参照
	static HeapReader& Ins() {
		static HeapReader hReader;
		return hReader;
	}

	//ヒープの確認　作成
	bool Create(D3D12_DESCRIPTOR_HEAP_TYPE, UINT, bool);

	//ヒープの取得
	ID3D12DescriptorHeap* GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE) const;

	//ヒープナンバーの取得
	int GetNum(D3D12_DESCRIPTOR_HEAP_TYPE) const;
};