#pragma once
#include <cassert>

#include <d3d12.h>

class RootSig
{
private:
	ID3D12RootSignature* rootShignature{};//ルートシグネチャー

public:
	RootSig() = default;//コンストラクタ
	~RootSig();			//デストラクタ

	//ルートシグネチャーの作成
	bool Create(ID3D12Device*);

	//ルートシグネチャーの取得
	ID3D12RootSignature* Get() { return rootShignature; }
};

