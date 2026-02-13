#pragma once

#include "Vertex.h"

namespace
{
	//ラスタライズステート　表裏面表示など
	D3D12_RASTERIZER_DESC Rasterizer()
	{
		D3D12_RASTERIZER_DESC rasterDesc{};
		rasterDesc.FillMode				 = D3D12_FILL_MODE_SOLID;	//塗りつぶす
		rasterDesc.CullMode				 = D3D12_CULL_MODE_NONE;	//表裏面表示判断
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.DepthBias			 = D3D12_DEFAULT_DEPTH_BIAS;
		rasterDesc.DepthBiasClamp		 = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterDesc.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterDesc.DepthClipEnable		 = true;		//深度方向のクリッピング
		rasterDesc.MultisampleEnable	 = false;		//アンチエイリアス
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.ForcedSampleCount	 = 0;
		rasterDesc.ConservativeRaster	 = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		return rasterDesc;
	}

	//ブレンド
	D3D12_BLEND_DESC Blend()
	{
		//ブレンドステート
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		blendDesc.BlendEnable = true;
		blendDesc.SrcBlend				= D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend				= D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.BlendOp				= D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha			= D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha		= D3D12_BLEND_ZERO;
		blendDesc.BlendOpAlpha			= D3D12_BLEND_OP_ADD;
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		D3D12_BLEND_DESC blend{};
		blend.AlphaToCoverageEnable  = false;	//アルファ対カバレッジ
		blend.IndependentBlendEnable = false;	//同時レンダーターゲットで独立させるか
		for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			blend.RenderTarget[i] = blendDesc;

		return blend;
	}

	//デプスステート
	D3D12_DEPTH_STENCIL_DESC Depth(bool depth)
	{
		D3D12_DEPTH_STENCIL_DESC depthDesc{};
		depthDesc.DepthEnable = depth;
		depthDesc.StencilEnable = false;
		depthDesc.DepthWriteMask = depth ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
		depthDesc.DepthFunc = depth ? D3D12_COMPARISON_FUNC_LESS : D3D12_COMPARISON_FUNC_NONE;

		return depthDesc;
	}
}