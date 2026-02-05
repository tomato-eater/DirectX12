#include "Shader.h"

#pragma comment(lib, "d3dcompiler.lib")

//デストラクタ
Shader::~Shader()
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
bool Shader::Create(std::string name)
{
	ID3DBlob* error{};
	const std::wstring temp = std::wstring(name.begin(), name.end());

	//頂点シェーダー
	auto hr = D3DCompileFromFile(temp.data(), nullptr, nullptr, "vs", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vs, &error);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "頂点シェーダコンパイルー失敗ー");
		return true;
	}

	//ピクセルシェーダー
	hr = D3DCompileFromFile(temp.data(), nullptr, nullptr, "ps", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &ps, &error);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "ピクセルシェーダコンパイルー失敗ー");
		return true;
	}

	if (error) error->Release();

	return false;
}