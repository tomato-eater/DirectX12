#pragma once

#include "PipTemp.h"

#include <wrl/client.h>

class PipLine
{
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipLineState{};	//パイプラインステート

public:
	//コンストラクタ　デストラクタ
	PipLine() = default;
	~PipLine() = default;

	//パイプラインステートの作成
	bool Create(ID3D12RootSignature*, ID3D10Blob*, ID3D10Blob*, bool, VerLay);

	//パイプラインステートの取得
	ID3D12PipelineState* Get() { return pipLineState.Get(); }
};

