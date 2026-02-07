#pragma once

#include <d3d12.h>

namespace
{
	//ラスタライズステート　表裏面表示など
	D3D12_RASTERIZER_DESC Rasterizer()
	{
		D3D12_RASTERIZER_DESC rasterDesc{};
		rasterDesc.FillMode				 = D3D12_FILL_MODE_SOLID;
		rasterDesc.CullMode				 = D3D12_CULL_MODE_NONE;	//表裏面表示判断
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.DepthBias			 = D3D12_DEFAULT_DEPTH_BIAS;
		rasterDesc.DepthBiasClamp		 = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterDesc.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterDesc.DepthClipEnable		 = true;
		rasterDesc.MultisampleEnable	 = false;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.ForcedSampleCount	 = 0;
		rasterDesc.ConservativeRaster	 = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		return rasterDesc;
	}

	//ブレンド
	D3D12_BLEND_DESC Blend(int idx)
	{
		//ブレンドステート
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		blendDesc.BlendEnable = true;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		D3D12_BLEND_DESC blend{};
		blend.AlphaToCoverageEnable  = false;	//アルファ対カバレッジ
		blend.IndependentBlendEnable = false;	//同時レンダーターゲットで独立させるか
		for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			blend.RenderTarget[i] = blendDesc;

		return blend;
	}

	//デプスステート
	D3D12_DEPTH_STENCIL_DESC Depth(bool type)
	{
		D3D12_DEPTH_STENCIL_DESC depthDesc{};
		depthDesc.DepthEnable = true;
		depthDesc.StencilEnable = false;
		depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

		return depthDesc;
	}
}