#pragma once

#include "SquarePoly.h"
#include "ConBuffer.h"

#include "Device.h"
#include "Heap.h"

#include "SquarePoly.h"
#include "Obj.h"
#include "ComLis.h"

#include <vector>
#include <math.h>

class Amo
{
private:	
	std::vector<bool> vacants{};
	std::vector<SquarePoly> pols{};
	std::vector<ConBuffer> buffs{};
	std::vector<Obj> objs{};

	DirectX::XMFLOAT3 tPos = { 0.0f, 1.5f, 5.0f };
	DirectX::XMFLOAT3 rPos = { 1.0f,-0.5f, 5.0f };
	DirectX::XMFLOAT3 lPos = {-1.0f,-0.5f, 5.0f };

public:
	Amo() = default;
	~Amo() = default;

	void Summon(Device& devi, Heap& heap, Obj& sqrObj, ComLis& list);

	void Update(ComLis& list);

	void Check(Obj& target);
};

