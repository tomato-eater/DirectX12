#pragma once

#include <DirectXMath.h>
#include <cmath>
#include <dinput.h>

class Obj
{
private:
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

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

	void SetRot(int);

	void Amo()
	{
		float x = DirectX::XMVectorGetX(world.r[3]);
		float y = DirectX::XMVectorGetY(world.r[3]);
		float z = DirectX::XMVectorGetZ(world.r[3]) + 0.02f;

		world = DirectX::XMMatrixTranslation(x, y, z);
	};

	void Hit();

	DirectX::XMMATRIX GetWorld() { return world; }
	DirectX::XMFLOAT4 GetColor() { return color; }

};

