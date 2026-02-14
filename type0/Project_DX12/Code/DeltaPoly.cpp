#include "DeltaPoly.h"
#include <cassert>

#include "Device.h"

//頂点バッファの作成
bool DeltaPoly::CreateVertex()
{
	//頂点データ
	Model3D::Vertex vertex[] = {
		{{-1.0f,-1.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f}},
		{{-1.0f, 1.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f}},
		{{ 1.0f,-1.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f}}
	};
	const auto size = sizeof(vertex);//データサイズ

	auto heap = SetHeap();			//ヒープをセット
	auto desc = SetResource(size);	//リソースをセット

	//バッファ作成
	if (Device::Ins().Get()->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff)) != S_OK)	{
		assert(false && "頂点バッファ作成ー失敗ー");
		return true;
	}

	Model3D::Vertex* data{};	//転送データを保管
	//マップ
	if (vertexBuff->Map(0, nullptr, reinterpret_cast<void**>(&data)) != S_OK) {
		assert(false && "頂点バッファのマップー失敗ー");
		return true;
	}
	memcpy_s(data, size, vertex, size);	//コピー
	vertexBuff->Unmap(0, nullptr);

	//ビュー
	vertexView.BufferLocation = vertexBuff->GetGPUVirtualAddress();
	vertexView.SizeInBytes = size;
	vertexView.StrideInBytes = sizeof(Model3D::Vertex);

	topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return false;
}

//インデックスバッファの作成
bool DeltaPoly::CreateIndex()
{
	//データの表示順
	UINT16 indice[] = { 0,1,2 };
	//データサイズ
	const auto size = sizeof(indice);

	auto heap = SetHeap();			//ヒープをセット
	auto desc = SetResource(size);	//リソースをセット

	//バッファの作成
	if (Device::Ins().Get()->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff))
		!= S_OK)	{
		assert(false && "インデックスバッファの作成ー失敗ー");
		return true;
	}

	UINT16* data{};
	//マップ
	if (indexBuff->Map(0, nullptr, reinterpret_cast<void**>(&data)) != S_OK) {
		assert(false && "インデックスバッファのマップー失敗ー");
		return true;
	}
	memcpy_s(data, size, indice, size);	//コピー
	vertexBuff->Unmap(0, nullptr);

	//ビュー
	indexView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	indexView.SizeInBytes = size;
	indexView.Format = DXGI_FORMAT_R16_UINT;

	count = _countof(indice);	//インデックス数登録
	return false;
}