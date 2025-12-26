#include "RootSig.h"

RootSig::~RootSig()
{
	if (rootSignature)
	{
		rootSignature->Release();
		rootSignature = nullptr;
	}
}

bool RootSig::Create(Device& devi)
{
	D3D12_DESCRIPTOR_RANGE r0{};
	r0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	r0.NumDescriptors = 1;
	r0.BaseShaderRegister = 0;
	r0.RegisterSpace = 0;
	r0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE r1{};
	r1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	r1.NumDescriptors = 1;
	r1.BaseShaderRegister = 1;
	r1.RegisterSpace = 0;
	r1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootPara[2]{};
	rootPara[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootPara[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootPara[0].DescriptorTable.NumDescriptorRanges = 1;
	rootPara[0].DescriptorTable.pDescriptorRanges = &r0;

	rootPara[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootPara[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootPara[1].DescriptorTable.NumDescriptorRanges = 1;
	rootPara[1].DescriptorTable.pDescriptorRanges = &r1;

	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.NumParameters = 2;
	desc.pParameters = rootPara;
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signature{};
	ID3DBlob* error{};
	auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

	bool success = SUCCEEDED(hr);
	if (!success)
	{
		assert(false && "ルートシグネチャーのシリアライズー失敗ー");
		if (signature) signature->Release();
		return true;
	}
	else
	{
		hr = devi.GetDev()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

		if (signature) signature->Release();

		success &= SUCCEEDED(hr);
		if (!success)
		{
			assert(false && "ルートシグネチャー生成ー失敗ー");
			return true;
		}
	}

	return false;
}