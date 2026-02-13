#pragma once

#include <d3d12.h>
#include <wrl/client.h>

class Fence
{
private:
	Microsoft::WRL::ComPtr<ID3D12Fence> fence{};	//フェンス
	HANDLE event{};			//イベント
	UINT value[2]{};		//今のフェンスの値
	UINT nextValue = 1;		//次のフェンスの値

public:
	//コンストラクタ　デストラクタ
	Fence() = default;
	~Fence() = default;

	//フェンス　イベント　の生成
	bool Create();

	//フェンスの確認
	void Check(UINT);

	//フェンス値の更新
	void Updata(ID3D12CommandQueue*, UINT);
};

