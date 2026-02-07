#pragma once
#include <cassert>

#include <d3d12.h>

//フルスクリーン描画
struct ScreenRoot
{
	ScreenRoot()
	{
		r0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		r0.NumDescriptors = 1;
		r0.BaseShaderRegister = 0;
		r0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		rPara[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rPara[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rPara[0].DescriptorTable.pDescriptorRanges = &r0;
		rPara[0].DescriptorTable.NumDescriptorRanges = 1;

		sample[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sample[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sample[0].MaxLOD = D3D12_FLOAT32_MAX;
		sample[0].MinLOD = 0.0f;
		sample[0].MipLODBias = 0;
		sample[0].MaxAnisotropy = 0;
		sample[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		sample[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sample[0].RegisterSpace = 0;
		sample[0].ShaderRegister = 0;
		sample[0].RegisterSpace = 0;

		desc.pParameters = rPara;
		desc.NumParameters = 1;
		desc.pStaticSamplers = sample;
		desc.NumStaticSamplers = 1;
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}
	D3D12_DESCRIPTOR_RANGE r0{};
	D3D12_ROOT_PARAMETER rPara[1]{};
	D3D12_STATIC_SAMPLER_DESC sample[1]{};
	D3D12_ROOT_SIGNATURE_DESC desc{};
};

class RootSig
{
private:
	ID3D12RootSignature* rootShignature{};//ルートシグネチャー

public:
	RootSig() = default;//コンストラクタ
	~RootSig();			//デストラクタ

	//ルートシグネチャーの作成
	bool Create(ID3D12Device*,const ScreenRoot&);

	//ルートシグネチャーの取得
	ID3D12RootSignature* Get() { return rootShignature; }
};

