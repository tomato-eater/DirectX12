#include "ScShader.h"

#pragma comment(lib, "d3dcompiler.lib")

ScShader::~ScShader()
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

bool ScShader::Create()
{
	ID3DBlob* error{};

	auto hr = D3DCompileFromFile(L"Screen.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs", "vs_5_0", 0, 0, &vertexShader, &error);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "Sc頂点シェーダコンパイルー失敗ー");
	}

	hr = D3DCompileFromFile(L"Screen.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps", "ps_5_0", 0, 0, &pixelShader, &error);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "Scピクセルシェーダコンパイルー失敗ー");
	}

	if (error) error->Release();
	return false;
}