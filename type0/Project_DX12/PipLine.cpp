#include "PipLine.h"

//デストラクタ
PipLine::~PipLine()
{
	if (pipLineState)
	{
		pipLineState->Release();
		pipLineState = nullptr;
	}
}

//パイプラインステートの作成
bool PipLine::Create(ELEMENTDESC layout, ID3D12Device* device, ID3D12RootSignature* rootSig, ID3DBlob* vs, ID3DBlob* ps, int idx, bool depth)
{
	//パイプラインステート
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graPipDesc{};
	graPipDesc.InputLayout = { layout.first, static_cast<UINT>(layout.second)};
	graPipDesc.pRootSignature = rootSig;
	graPipDesc.VS = { vs->GetBufferPointer(),vs->GetBufferSize() };
	graPipDesc.PS = { ps->GetBufferPointer(),ps->GetBufferSize() };
	graPipDesc.RasterizerState = Rasterizer();	//ポリゴンの描画設定
	graPipDesc.BlendState = Blend(idx);
	graPipDesc.DepthStencilState = Depth(depth);
	graPipDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	graPipDesc.SampleMask = UINT_MAX;
	graPipDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graPipDesc.NumRenderTargets = 1;
	graPipDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	graPipDesc.SampleDesc = { 1,0 };
	device->CreateGraphicsPipelineState(
		&graPipDesc,					//上記のやつ
		IID_PPV_ARGS(&pipLineState));	//ぶちこむ
	if (!pipLineState) {
		assert(false && "パイプラインステート作成ー失敗ー");
		return true;
	}

	return false;
}