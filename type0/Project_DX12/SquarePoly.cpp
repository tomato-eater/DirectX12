#include "SquarePoly.h"

SquarePoly::~SquarePoly()
{
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

//頂点　インデックス　の生成
bool SquarePoly::Create(ID3D12Device* device)
{
	//頂点
	{
		VertexTemp::Vertex vertices[] = //座標と色
		{
			{{-1.0f, 1.0f, 0.0f},{0.0f,0.0f}},
			{{ 1.0f, 1.0f, 0.0f},{1.0f,0.0f}},
			{{-1.0f,-1.0f, 0.0f},{0.0f,1.0f}},
			{{ 1.0f,-1.0f, 0.0f},{1.0f,1.0f}}
		};
		auto size = sizeof(vertices);

		auto heap = VertexTemp::SetHeap();
		auto desc = VertexTemp::SetDesc(size);

		//リソース作成
		if (device->CreateCommittedResource(
			&heap,								//上で作ったヒープ
			D3D12_HEAP_FLAG_NONE,				//フラッグ
			&desc,								//上で作ったデスク
			D3D12_RESOURCE_STATE_GENERIC_READ,	//リソース状態
			nullptr,							//リセット値(?)
			IID_PPV_ARGS(&vertexBuffer)))		//頂点バッファにぶち込む
		{
			assert(false && "頂点バッファ作成ー失敗ー");
			return true;
		}

		//マップの確認
		VertexTemp::Vertex* data{};
		vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
		if (!data) {
			assert(false && "頂点バッファのマップー失敗ー");
			return true;
		}
		memcpy_s(data, size, vertices, size);
		vertexBuffer->Unmap(0, nullptr);

		//成功したデータをぶち込む
		vertexView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexView.SizeInBytes = size;
		vertexView.StrideInBytes = sizeof(VertexTemp::Vertex);
	}

	//インデックス
	{
		//上のverticesをどの順番で描画するか
		unsigned short indices[] = { 0,1,2,3 };
		indexSize = sizeof(indices);

		auto heap = VertexTemp::SetHeap();
		auto desc = VertexTemp::SetDesc(indexSize);

		//リソースの作成
		if (device->CreateCommittedResource(
			&heap,								//上で作ったヒープ
			D3D12_HEAP_FLAG_NONE,				//フラッグ
			&desc,								//上で作ったデスク
			D3D12_RESOURCE_STATE_GENERIC_READ,	//リソース状態
			nullptr,							//リセット値(?)
			IID_PPV_ARGS(&indexBuffer)))		//インデックスバッファにぶち込む
		{
			assert(false && "インデックスバッファ作成ー失敗ー");
			return true;
		}

		//マップの確認
		unsigned short* data{};
		indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
		if (!data) {
			assert(false && "インデックスバッファのマップー失敗ー");
			return true;
		}
		memcpy_s(data, indexSize, indices, indexSize);
		indexBuffer->Unmap(0, nullptr);

		//成功したデータをぶち込む
		indexView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexView.SizeInBytes = indexSize;
		indexView.Format = DXGI_FORMAT_R16_UINT;
	}

	return false;
}

//描画
void SquarePoly::Drow(ID3D12GraphicsCommandList* list)
{
	list->IASetVertexBuffers(0, 1, &vertexView);
	list->IASetIndexBuffer(&indexView);
	list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	list->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}