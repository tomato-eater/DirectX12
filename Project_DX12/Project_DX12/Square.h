#pragma once

#include "SquarePoly.h"
#include "ConBuffer.h"
#include "Obj.h"

#include "Device.h"
#include "Heap.h"
#include "ComLis.h"

class Square
{
private:
	SquarePoly poly{};
	ConBuffer buff{};
	Obj obj{};

public:
	bool Create(Device&, Heap&, UINT);
	void Set(DirectX::XMFLOAT3, DirectX::XMFLOAT4);
	void Update(ComLis&);

	Obj& GetObj() { return obj; }
};

