#include "ScRootSig.h"

ScRootSig::~ScRootSig()
{
	if (rootSignature)
	{
		rootSignature->Release();
		rootSignature = nullptr;
	}
}

bool ScRootSig::Create(Device& devi)
{
	D3D12_DESCRIPTOR_RANGE r0{};
	r0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	r0.NumDescriptors = 1;
	r0.BaseShaderRegister = 0;
	r0.RegisterSpace = 0;
	r0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootPara[1]{};
	rootPara[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootPara[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootPara[0].DescriptorTable.NumDescriptorRanges = 1;
	rootPara[0].DescriptorTable.pDescriptorRanges = &r0;

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.ShaderRegister = 0;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.NumParameters = 1;
	desc.pParameters = rootPara;
	desc.pStaticSamplers = &samplerDesc;
	desc.NumStaticSamplers = 1;
	desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signature{};
	ID3DBlob* error{};
	auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	bool success = SUCCEEDED(hr);
	if(!success)
	{
		assert(false && "Scルートシグネチャーのシリアライズー失敗ー");
		if (signature) signature->Release();
		return true;
	}
	else {
		hr = devi.GetDev()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		if (signature) signature->Release();
		success &= SUCCEEDED(hr);
		if (!success)
		{
			assert(false && "Scルートシグネチャー作成ー失敗ー");
			return true;
		}
	}
	return false;
}