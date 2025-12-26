#pragma once

#include "Device.h"
#include "RootSig.h"
#include "Shader.h"

class PipLine
{
private:
	ID3D12PipelineState* pipLineState = {};

public:
	PipLine() = default;
	~PipLine();

	bool Create(Device& devi, RootSig& root, Shader& shader);

	ID3D12PipelineState* GetPip() { return pipLineState; }
};

