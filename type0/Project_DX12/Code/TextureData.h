#pragma once
#include<cassert>

#include <DirectXTex.h>
#include <d3d12.h>
#include <string>

class TextureData
{
private:
	DirectX::TexMetadata metadata{};	//メタデータ
	DirectX::ScratchImage scImage{};	//イメージ

	ID3D12Resource* texResource{};			//バッファ

public:
	TextureData() = default;	//コンストラクタ
	~TextureData();				//デストラクタ

	//テクスチャの登録　バッファの作成
	bool Create(std::string, ID3D12Device*);

	//シェーダーリソースビューの作成
	void Set(ID3D12DescriptorHeap*, ID3D12Device*);

	//バッファの取得
	ID3D12Resource* GetRes() { return texResource; }
};

