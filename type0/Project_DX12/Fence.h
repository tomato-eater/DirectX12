#pragma once
#include <cassert>

#include <d3d12.h>

class Fence
{
private:
	ID3D12Fence* fence{};	//フェンス
	HANDLE event{};			//イベント
	UINT value[2]{};		//今のフェンスの値
	UINT nextValue = 1;		//次のフェンスの値

public:
	Fence() = default;	//コンストラクタ
	~Fence();			//デストラクタ

	//フェンス　イベント　の生成
	bool Create(ID3D12Device*);

	//フェンスの確認
	void Check(UINT);

	//フェンス値の更新
	void Updata(ID3D12CommandQueue*, UINT);
};

