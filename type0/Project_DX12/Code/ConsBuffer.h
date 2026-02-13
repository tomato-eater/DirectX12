#pragma once

#include <wrl/client.h>
#include <d3d12.h>

//コンスタントバッファクラス
class ConsBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> consbuffer{};	//コンスタントバッファ
	D3D12_GPU_DESCRIPTOR_HANDLE handle{};	//GPUハンドル

public:
	//コンストラクタ　デストラクタ
	ConsBuffer() = default;
	~ConsBuffer() = default;

	//コンスタントバッファ
	bool Create(UINT);

	//コンスタントバッファの取得
	ID3D12Resource* GetBuff() const { return consbuffer.Get(); }

	//ハンドルの取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() const { return handle; }
};