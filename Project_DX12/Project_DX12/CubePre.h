#pragma once

#include "CubePoly.h";
#include "ConBuffer.h"
#include "Obj.h"

#include "Device.h"
#include "Heap.h"
#include "ComLis.h"

#include "PosPro.h"

#include <math.h>

class CubePre
{
private:
	bool vacants[25]{};
	CubePoly pols[25]{};
	ConBuffer buffs[25]{};
	Obj objs[25]{};

	DirectX::XMFLOAT3 tPos = { 0.0f, 1.5f, 5.0f };
	DirectX::XMFLOAT3 rPos = { 1.0f,-0.5f, 5.0f };
	DirectX::XMFLOAT3 lPos = { -1.0f,-0.5f, 5.0f };

public:
	CubePre() = default;
	~CubePre() = default;

	//void Summon(Device& devi, Heap& heap, Obj& sqrObj, ComLis& list);
	void Summon(Device& devi, PosPro&, Obj& sqrObj, ComLis& list);

	void Update(ComLis& list);

	void Check(Obj& target);
};

