#pragma once

#include <DirectXMath.h>
#include <cmath>
#include <dinput.h>

class Obj
{
	private:
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		float x{};
		float y{};
		float z{};

public:
	struct ConstBufferData
	{
		DirectX::XMMATRIX world{};
		DirectX::XMFLOAT4 color{};
	};

	Obj() = default;
	~Obj() = default;

	void Initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color);

	void Updata();

	void Amo()
	{
		z += 0.02f;
		world = DirectX::XMMatrixTranslation(x, y, z);
	};

	void Hit();

	DirectX::XMMATRIX GetWorld() { return world; }
	DirectX::XMFLOAT4 GetColor() { return color; }

	DirectX::XMFLOAT3 GetPos() { return DirectX::XMFLOAT3(x, y, z); }
};

