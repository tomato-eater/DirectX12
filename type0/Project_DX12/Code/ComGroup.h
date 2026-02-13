#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

/// <summary>
/// コマンド_アロケータ リスト キュー 等のグループ化クラス
/// </summary>
class ComGroup
{
private:
	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> allocacators;	//コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> list;						//コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue;							//コマンドキュー

public:
	//コンストラクタ　デストラク
	ComGroup() = default;
	~ComGroup() { allocacators.clear(); }

	//コマンド_アロケータ リスト キュー　作成
	bool Create(UINT, D3D12_COMMAND_LIST_TYPE);
	
	//コマンドアロケータの取得
	ID3D12CommandAllocator* Allo(UINT i = 0) const { return allocacators[i].Get(); }
	//コマンドリストの取得
	ID3D12GraphicsCommandList* List() const { return list.Get(); }
	//コマンドキューの取得
	ID3D12CommandQueue* Queue() const { return queue.Get(); }


	//レンダ―ターゲットを各々変更
	void ResourceBarrier(ID3D12Resource*, D3D12_RESOURCE_STATES, D3D12_RESOURCE_STATES);

	//ビューポート　シザー　の設定
	void SetVS(float, float);

	//アロケータ　リスト　のリセット
	void Resets(UINT);
};

