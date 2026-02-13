#pragma once

#include <string>
#include <d3dcompiler.h>
#include <wrl/client.h>

class Shader
{
private:
	Microsoft::WRL::ComPtr<ID3DBlob> vs{};//頂点シェーダー
	Microsoft::WRL::ComPtr<ID3DBlob> ps{};//ピクセルシェーダー

public:
	//コンストラクタ　デストラクタ
	Shader() = default;	
	~Shader() = default;

	//シェーダーの読み込み
	bool Create(std::string);

	//頂点シェーダーを渡す
	ID3DBlob* VS() { return vs.Get(); }

	//ピクセルシェーダーを渡す
	ID3DBlob* PS() { return ps.Get(); }
};

