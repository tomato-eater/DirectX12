#pragma once

#include "Device.h"
#include "ComAll.h"

#include "Target.h"

class ComLis
{
private:
	ID3D12GraphicsCommandList* commandList{};

public:
	ComLis() = default;
	~ComLis();

	bool Create(Device& devi, ComAll& comAll);

	void Reset(ComAll& comAll) { commandList->Reset(comAll.GetAll(), nullptr); }

	void Chenge(Target& tar, UINT idx, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to);
	void ScChenge(ID3D12Resource*, UINT idx, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to);

	ID3D12GraphicsCommandList* GetList() { return commandList; }

	void SetVS(int wi, int hi);
};

