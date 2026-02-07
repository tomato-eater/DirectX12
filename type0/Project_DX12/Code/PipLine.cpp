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

/// <summary>
/// パイプラインステートの作成
/// 0 :頂点レイアウト
/// </summary>
/// <param name="layNo"></param>
/// <param name="device"></param>
/// <param name="rootSig"></param>
/// <param name="vs"></param>
/// <param name="ps"></param>
/// <param name="idx"></param>
/// <param name="depth"></param>
/// <returns></returns>
bool PipLine::Create(int layNo, ID3D12Device* device, ID3D12RootSignature* rootSig, ID3DBlob* vs, ID3DBlob* ps, int idx, bool depth)
{
	std::pair<D3D12_INPUT_ELEMENT_DESC*, size_t> layout{};
	switch (layNo)
	{
		case 0: {	//頂点レイアウト
			D3D12_INPUT_ELEMENT_DESC layoutDesc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};
			layout = { layoutDesc, _countof(layoutDesc) };
		}
		break;

		case 1: {

		}
			break;
	}

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
	auto hr = device->CreateGraphicsPipelineState(
		&graPipDesc,					//上記のやつ
		IID_PPV_ARGS(&pipLineState));	//ぶちこむ
	if (!pipLineState) {
		assert(false && "パイプラインステート作成ー失敗ー");
		return true;
	}

	return false;
}