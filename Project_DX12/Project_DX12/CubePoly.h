#pragma once

#include "Device.h"
#include "ComLis.h"

#include <DirectXMath.h>

class CubePoly
{
	private:
		ID3D12Resource* vertexBuffer{};
		ID3D12Resource* indexBuffer{};

		D3D12_VERTEX_BUFFER_VIEW vertexView = {};
		D3D12_INDEX_BUFFER_VIEW indexView = {};

		void SetHeap(D3D12_HEAP_PROPERTIES& heap);
		void SetDesc(D3D12_RESOURCE_DESC& desc, UINT size);

public:
	struct ConstBufferData
	{
		DirectX::XMMATRIX world{};
		DirectX::XMFLOAT4 color{};
	};

	CubePoly() = default;
	~CubePoly();

	bool Create(Device& devi);
	void Draw(ComLis& comLis);
};

