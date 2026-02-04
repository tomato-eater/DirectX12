#pragma once
#include <cassert>

#include <d3d12.h>

/// <summary>
/// コマンドアロケータ
/// コマンドリスト
/// コマンドキュー
/// 等のグループ化クラス
/// </summary>
class ComGroup
{
private:
	ID3D12CommandAllocator* commandAllocator;	//コマンドアロケータ
	ID3D12GraphicsCommandList* commandList;		//コマンドリスト
	ID3D12CommandQueue* commandQueue;			//コマンドキュー

public:
	ComGroup() = default;//コンストラクタ
	~ComGroup();		 //デストラクタ

	//コマンドアロケータ　コマンドリスト　コマンドキュー　作成
	bool Create(ID3D12Device*, D3D12_COMMAND_LIST_TYPE);
	
	//コマンドアロケータの取得
	ID3D12CommandAllocator* GetAllo() const { return commandAllocator; }
	//コマンドリストの取得
	ID3D12GraphicsCommandList* GetList() const { return commandList; }
	//コマンドキューの取得
	ID3D12CommandQueue* GetQueue() const { return commandQueue; }

	//レンダ―ターゲットを各々変更
	void ResourceBarrier(ID3D12Resource*, D3D12_RESOURCE_STATES, D3D12_RESOURCE_STATES);

	//アロケータ　リスト　のリセット
	void Resets(UINT);
};

