#pragma once

#include <wrl/client.h>
#include "Vertex.h"

//ポリゴンのテンプレート
class PolyTemp
{
private:
public:
	//コンストラクタ　デストラクタ
	PolyTemp() = default;
	virtual ~PolyTemp() = default;

	//ポリゴンの作成
	bool Create();


	//ポリゴンの描画
	void Drow(ID3D12GraphicsCommandList*);

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuff{};//頂点バッファー
	D3D12_VERTEX_BUFFER_VIEW vertexView{};				//頂点バッファービュー
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff{};	//インデックスバッファー
	D3D12_INDEX_BUFFER_VIEW indexView{};				//インデックスバッファービュー
	UINT count{};									//インデックス数
	D3D_PRIMITIVE_TOPOLOGY	topolory{};					//描画タイプ

	//頂点バッファの作成
	virtual bool CreateVertex() {};

	//インデックスバッファの作成
	virtual bool CreateIndex() {};

	//ヒープの設定
	virtual D3D12_HEAP_PROPERTIES SetHeap();

	//リソースの設定
	virtual D3D12_RESOURCE_DESC SetResource(const UINT);
};