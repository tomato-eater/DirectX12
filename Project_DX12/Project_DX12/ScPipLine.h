#pragma once

#include "Device.h"
#include "ScRootSig.h"
#include "ScShader.h"

class ScPipLine
{
private:
	ID3D12PipelineState* pipLineState{};

public:
	ScPipLine() = default;
	~ScPipLine();

	bool Create(Device& devi, ScRootSig& root, ScShader& shader);
	ID3D12PipelineState* GetPip() { return pipLineState; }

};

