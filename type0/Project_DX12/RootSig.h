#pragma once
#include <cassert>

#include <d3d12.h>

class RootSig
{
private:
	ID3D12RootSignature* rootShignature{};//ルートシグネイチャー

public:
	RootSig() = default;//コンストラクタ
	~RootSig();			//デストラクタ

	//ルートシグネイチャーの作成
	bool Create(ID3D12Device*);

	//ルートシグネイチャーの取得
	ID3D12RootSignature* Get() { return rootShignature; }
};

