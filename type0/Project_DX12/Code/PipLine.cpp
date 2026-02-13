#include "PipLine.h"
#include <cassert>

#include "Device.h"

// パイプラインステートの作成　描画方法等
bool PipLine::Create(ID3D12RootSignature* root, ID3D10Blob* vs, ID3D10Blob* ps, bool depth, VerLay lay)
{
	//パイプラインステート
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graPipDesc{};
	graPipDesc.InputLayout = { lay.first, static_cast<UINT>(lay.second) };
	graPipDesc.pRootSignature = root;
	graPipDesc.VS = { vs->GetBufferPointer(),vs->GetBufferSize() };
	graPipDesc.PS = { ps->GetBufferPointer(),ps->GetBufferSize() };
	graPipDesc.RasterizerState = Rasterizer();	//ポリゴンの描画設定
	graPipDesc.BlendState = Blend();
	graPipDesc.DepthStencilState = Depth(depth);
	graPipDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	graPipDesc.SampleMask = UINT_MAX;
	graPipDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graPipDesc.NumRenderTargets = 1;
	graPipDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	graPipDesc.SampleDesc = { 1,0 };
	if (S_OK != Device::Ins().Get()->CreateGraphicsPipelineState(
		&graPipDesc,					//上記のやつ
		IID_PPV_ARGS(&pipLineState)))	//ぶちこむ
	{
		assert(false && "パイプラインステート作成ー失敗ー");
		return true;
	}
	return false;
}