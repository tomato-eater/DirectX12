#include "ScPipLine.h"

ScPipLine::~ScPipLine()
{
	if (pipLineState)
	{
		pipLineState->Release();
		pipLineState = nullptr;
	}
}

D3D12_RASTERIZER_DESC Rasterzer() 
{
	D3D12_RASTERIZER_DESC rDesc{};
	rDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rDesc.CullMode = D3D12_CULL_MODE_NONE;
	rDesc.FrontCounterClockwise = false;
	rDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rDesc.DepthClipEnable = true;
	rDesc.MultisampleEnable = false;
	rDesc.AntialiasedLineEnable = false;
	rDesc.ForcedSampleCount = 0;
	rDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	return rDesc;
}

D3D12_BLEND_DESC Blend()
{
	D3D12_RENDER_TARGET_BLEND_DESC rtbDesc{};
	rtbDesc.BlendEnable = false;
	rtbDesc.LogicOpEnable = false;
	rtbDesc.SrcBlend = D3D12_BLEND_ONE;
	rtbDesc.DestBlend = D3D12_BLEND_ZERO;
	rtbDesc.BlendOp = D3D12_BLEND_OP_ADD;
	rtbDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	rtbDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	rtbDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	rtbDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	rtbDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_BLEND_DESC bDesc{};
	bDesc.AlphaToCoverageEnable = false;
	bDesc.IndependentBlendEnable = false;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		bDesc.RenderTarget[i] = rtbDesc;
	return bDesc;
}

D3D12_DEPTH_STENCIL_DESC DepthStencil()
{
	D3D12_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = false;
	dsDesc.StencilEnable = false;
	dsDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	return dsDesc;
}

bool ScPipLine::Create(Device& devi, ScRootSig& root, ScShader& shader)
{
	D3D12_INPUT_ELEMENT_DESC descs[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc{};
	gpsDesc.InputLayout = { descs, _countof(descs) };
	gpsDesc.pRootSignature = root.GetSign();
	gpsDesc.VS = { shader.GetVer()->GetBufferPointer(), shader.GetVer()->GetBufferSize() };
	gpsDesc.PS = { shader.GetPix()->GetBufferPointer(), shader.GetPix()->GetBufferSize() };
	gpsDesc.RasterizerState = Rasterzer();
	gpsDesc.BlendState = Blend();
	gpsDesc.DepthStencilState = DepthStencil();
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.SampleDesc = { 1, 0 };
	gpsDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	auto hr = devi.GetDev()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&pipLineState));
	if(FAILED(hr))
	{
		assert(false && "Scパイプラインステート作成ー失敗ー");
		return true;
	}
	return false;
}