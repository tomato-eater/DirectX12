#pragma once

#include "DeltaPoly.h"
#include "ConBuffer.h"
#include "Obj.h"

#include "Device.h"
#include "Heap.h"
#include "ComLis.h"

#include "PosPro.h"

class Delta
{
private:
	DeltaPoly poly{};	//ポリゴン
	ConBuffer buff{};	//バッファ
	Obj obj{};			//オブジェクト

public:
	//bool Create(Device& devi, Heap& cons, UINT idx);
	bool Create(Device& devi, PosPro&, UINT idx);
	void Set(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color);
	void Update(ComLis& lis);

	Obj& GetObj() { return obj; }
};

