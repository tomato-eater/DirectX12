#include "PipLine.h"

PipLine::~PipLine()
{
	if (pipLineState)
	{
		pipLineState->Release();
		pipLineState = nullptr;
	}
}

bool PipLine::Create(Device& devi, RootSig& root, Shader& shader)
{
	D3D12_INPUT_ELEMENT_DESC descs[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	D3D12_RASTERIZER_DESC rDesc{};
	rDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rDesc.CullMode = D3D12_CULL_MODE_BACK;
	rDesc.FrontCounterClockwise = false;
	rDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rDesc.DepthClipEnable = true;
	rDesc.MultisampleEnable = false;
	rDesc.AntialiasedLineEnable = false;
	rDesc.ForcedSampleCount = 0;
	rDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	D3D12_RENDER_TARGET_BLEND_DESC rtbDesc{};
	rtbDesc.BlendEnable = true;
	rtbDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	rtbDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	rtbDesc.BlendOp = D3D12_BLEND_OP_ADD;
	rtbDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	rtbDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	rtbDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	rtbDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_BLEND_DESC bDesc{};
	bDesc.AlphaToCoverageEnable = false;
	bDesc.IndependentBlendEnable = false;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		bDesc.RenderTarget[i] = rtbDesc;

	D3D12_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = true;
	dsDesc.StencilEnable = false;
	dsDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc{};
	gpsDesc.InputLayout = { descs, _countof(descs) };
	gpsDesc.pRootSignature = root.GetSign();
	gpsDesc.VS = { shader.GetVer()->GetBufferPointer(), shader.GetVer()->GetBufferSize() };
	gpsDesc.PS = { shader.GetPix()->GetBufferPointer(), shader.GetPix()->GetBufferSize() };
	gpsDesc.RasterizerState = rDesc;
	gpsDesc.BlendState = bDesc;
	gpsDesc.DepthStencilState = dsDesc;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.SampleMask = UINT_MAX;
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.SampleDesc = { 1, 0 };

	auto hr =  devi.GetDev()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&pipLineState));
	if (FAILED(hr))
	{
		assert(false && "パイプラインステート作成ー失敗ー");
		return true;
	}

	return false;
}