#include "Shader.h"

#pragma comment(lib, "d3dcompiler.lib")

Shader::~Shader()
{
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}
}

bool Shader::Create(Device& devi)
{
	ID3DBlob* error{};

	auto hr = D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "vs", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &error);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "頂点シェーダコンパイルー失敗ー");
	}

	hr = D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "ps", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &error);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "ピクセルシェーダコンパイルー失敗ー");
	}

	if (error) error->Release();

	return false;
}