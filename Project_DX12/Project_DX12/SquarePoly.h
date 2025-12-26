#pragma once

#include "Device.h"
#include "ComLis.h"

#include <DirectXMath.h>

class SquarePoly
{
private:
	ID3D12Resource* vertexBuffer{};
	ID3D12Resource* indexBuffer{};

	D3D12_VERTEX_BUFFER_VIEW vertexView = {};
	D3D12_INDEX_BUFFER_VIEW indexView = {};

public:
	struct ConstBufferData
	{
		DirectX::XMMATRIX world{};
		DirectX::XMFLOAT4 color{};
	};

	SquarePoly() = default;
	~SquarePoly();

	bool Create(Device& devi, bool amo = false);

	void Draw(ComLis& comLis);
};

