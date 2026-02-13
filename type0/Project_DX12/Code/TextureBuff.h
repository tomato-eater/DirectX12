#pragma once
#include <cassert>

#include <DirectXMath.h>
#include <utility>
#include <d3d12.h>

class TextureBuff
{
private:
	DirectX::XMMATRIX matrix{};
	DirectX::XMMATRIX worldMat{};

	DirectX::XMFLOAT3 eye{};
	DirectX::XMFLOAT3 target{};
	DirectX::XMFLOAT3 up{};

	ID3D12Resource* constBuff;

public:
	TextureBuff() = default;	//コンストラクタ
	~TextureBuff();				//デストラクタ
	
	//3D空間への準備　バッファの作成
	bool Create(std::pair<float,float>,ID3D12Device*, ID3D12DescriptorHeap*);

	ID3D12Resource* GetBuff() { return constBuff; }
};

