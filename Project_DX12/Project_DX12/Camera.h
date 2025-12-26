#pragma once

#include <DirectXMath.h>
#include <cmath>

#include <dinput.h>

class Camera
{
private:
	DirectX::XMMATRIX view{};
	DirectX::XMMATRIX projection{};

	DirectX::XMFLOAT3 position{};
	DirectX::XMFLOAT3 target{};
	DirectX::XMFLOAT3 up{};

public:
	struct ConstBufferData
	{
		DirectX::XMMATRIX view{};
		DirectX::XMMATRIX projection{};
	};

	Camera() = default;
	~Camera() = default;

	void Initialize(int wi, int hi);

	void Updata();

	DirectX::XMMATRIX GetView() { return view; }
	DirectX::XMMATRIX GetPro() { return projection; }
};

