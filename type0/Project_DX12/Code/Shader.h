#pragma once
#include <cassert>

#include <string>
#include <d3dcompiler.h>

class Shader
{
private:
	ID3DBlob* vs{};//頂点シェーダー
	ID3DBlob* ps{};//ピクセルシェーダー

public:
	Shader() = default;	//コンストラクタ
	~Shader();			//デストラクタ

	//シェーダーの読み込み
	bool Create(std::string);

	//頂点シェーダーを渡す
	ID3DBlob* VS() { return vs; }

	//ピクセルシェーダーを渡す
	ID3DBlob* PS() { return ps; }
};

