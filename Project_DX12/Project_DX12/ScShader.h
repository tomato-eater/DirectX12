#pragma once

#include <D3Dcompiler.h>
#include <cassert>

class ScShader
{
private:
	ID3DBlob* vertexShader{};
	ID3DBlob* pixelShader{};

public:
	ScShader() = default;
	~ScShader();

	bool Create();

	ID3DBlob* GetVer() { return vertexShader; }
	ID3DBlob* GetPix() { return pixelShader; }
};

