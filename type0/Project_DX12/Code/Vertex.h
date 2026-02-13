#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <utility>

using VerLay = std::pair<D3D12_INPUT_ELEMENT_DESC*, size_t>;

struct Model3D
{
	struct Vertex {
		DirectX::XMFLOAT3 position;	//頂点座標
		DirectX::XMFLOAT4 color;	//頂点カラー
	};
	static VerLay Layout() {
		static D3D12_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION",0,   DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
			{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		};
		return { layout,_countof(layout) };
	}
};