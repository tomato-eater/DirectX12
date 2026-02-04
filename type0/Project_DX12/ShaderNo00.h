#pragma once
#include <cassert>

#include <d3dcompiler.h>

class ShaderNo00
{
private:
	ID3DBlob* vs{};//頂点シェーダー
	ID3DBlob* ps{};//ピクセルシェーダー

public:
	ShaderNo00() = default;	//コンストラクタ
	~ShaderNo00();			//デストラクタ

	//シェーダーの読み込み
	bool Create();

	//頂点シェーダーを渡す
	ID3DBlob* VS() { return vs; }

	//ピクセルシェーダーを渡す
	ID3DBlob* PS() { return ps; }
};

