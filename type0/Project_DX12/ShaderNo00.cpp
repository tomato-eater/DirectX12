#include "ShaderNo00.h"

#pragma comment(lib, "d3dcompiler.lib")

//デストラクタ
ShaderNo00::~ShaderNo00()
{
	if (vs)
	{
		vs->Release();
		vs = nullptr;
	}
	if (ps)
	{
		ps->Release();
		ps = nullptr;
	}
}

//シェーダーの読み込み
bool ShaderNo00::Create()
{
	ID3DBlob* error{};

	//頂点シェーダー
	auto hr = D3DCompileFromFile(L"Shader00.hlsl", nullptr, nullptr, "vs", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vs, &error);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "頂点シェーダコンパイルー失敗ー");
	}

	//ピクセルシェーダー
	hr = D3DCompileFromFile(L"Shader00.hlsl", nullptr, nullptr, "ps", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &ps, &error);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "ピクセルシェーダコンパイルー失敗ー");
	}

	if (error) error->Release();

	return false;
}