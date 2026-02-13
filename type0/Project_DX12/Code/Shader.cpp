#include "Shader.h"
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

//シェーダーの読み込み
bool Shader::Create(std::string name)
{
	Microsoft::WRL::ComPtr<ID3DBlob> error{};
	const std::wstring temp = std::wstring(name.begin(), name.end());

	//頂点シェーダー
	if (D3DCompileFromFile(
		temp.data(),
		nullptr,
		nullptr,
		"vs", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vs,
		&error)
		!= S_OK)
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "頂点シェーダコンパイルー失敗ー");
		return true;
	}

	//ピクセルシェーダー
	if (D3DCompileFromFile(
		temp.data(),
		nullptr,
		nullptr,
		"ps", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&ps,
		&error)
		!= S_OK)
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "ピクセルシェーダコンパイルー失敗ー");
		return true;
	}

	return false;
}