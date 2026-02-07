#pragma once
#include <cassert>

#include "PipTemp.h"

#include <utility>

class PipLine
{
private:
	ID3D12PipelineState* pipLineState{};	//パイプラインステート

public:
	PipLine() = default;	//コンストラクタ
	~PipLine();				//デストラクタ

	//パイプラインステートの作成
	bool Create(int, ID3D12Device*, ID3D12RootSignature*, ID3DBlob*, ID3DBlob*, int, bool);

	//パイプラインステートの取得
	ID3D12PipelineState* Get() { return pipLineState; }
};

