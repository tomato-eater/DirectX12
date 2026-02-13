#pragma once
#include "PolyTemp.h"

class DeltaPoly : public PolyTemp
{
private:
	//頂点バッファ作成
	virtual bool CreateVertex();

	//インデックスバッファ作成
	virtual bool CreateIndex();

public:
	//コンストラクタ　デストラクタ
	DeltaPoly() = default;
	~DeltaPoly() = default;
};

