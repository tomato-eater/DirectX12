#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <vector>
#include <wrl/client.h>
#include <utility>

class RendTarget
{
private:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> renderTargets{}; //レンダーターゲット

	//レンダービューの作成
	bool CreateRender();

	//シェーダービューの作成
	bool CreateShader();

public:
	//コンストラクタ　デストラクタ
	RendTarget() = default;
	~RendTarget() { renderTargets.clear(); }

	//レンダーターゲットの作成
	bool Create(const DXGI_SWAP_CHAIN_DESC1&,IDXGISwapChain4*);

	//レンダーターゲットの取得
	ID3D12Resource* Get(UINT index) const { return renderTargets[index].Get(); }

	//レンダ―ターゲットのサイズ取得
	std::pair<float, float> Size() const;

	//ディスクリプタハンドルRTVの取得
	D3D12_CPU_DESCRIPTOR_HANDLE RTVhandle(UINT) const;
};