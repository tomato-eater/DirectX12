#pragma once
#include <cassert>

#include "Vertex.h"

class SquarePoly
{
private:
	ID3D12Resource* vertexBuffer{};			//頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vertexView{};	//頂点ビュー

	ID3D12Resource* indexBuffer{};			//インデックスバッファ
	D3D12_INDEX_BUFFER_VIEW indexView{};	//インデックスビュー

	UINT indexSize{};						//インデックスバッファのサイズ

public:
	SquarePoly() = default;	//コンストラクタ
	~SquarePoly();			//デストラクタ

	//頂点　インデックス　の生成
	bool Create(ID3D12Device*);

	//描画
	void Drow(ID3D12GraphicsCommandList*);
};