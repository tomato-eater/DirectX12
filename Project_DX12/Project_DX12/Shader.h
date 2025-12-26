#pragma once

#include "Device.h"

#include <D3Dcompiler.h>

class Shader
{
private:
	ID3DBlob* vertexShader{};
	ID3DBlob* pixelShader{};

public:
	Shader() = default;
	~Shader();

	bool Create(Device& devi);

	ID3DBlob* GetVer() { return vertexShader; }
	ID3DBlob* GetPix() { return pixelShader; }
};

