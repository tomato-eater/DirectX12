#include "TextureData.h"

#pragma comment(lib, "DirectXTex.lib")

//デストラクタ
TextureData::~TextureData()
{
	if (texResource)
	{
		texResource->Release();
		texResource = nullptr;
	}
}

//テクスチャの登録　バッファの作成
bool TextureData::Create(std::string name, ID3D12Device* device)
{	
	//テクスチャの登録
	const std::wstring temp = std::wstring(name.begin(), name.end());
	auto hr = DirectX::LoadFromWICFile(
		temp.data(),			//ファイルの場所
		DirectX::WIC_FLAGS_NONE,//フラグ
		&metadata,				//メタデータ
		scImage);				//イメージ
	if (FAILED(hr)) {
		assert(false && "テクスチャの読み込みー失敗ー");
		return true;
	}

	//バッファ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;		//テクスチャの場合、カスタムが最適らしい
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Format = metadata.format;
	resourceDesc.Width = metadata.width;
	resourceDesc.Height = metadata.height;
	resourceDesc.DepthOrArraySize = metadata.arraySize;
	resourceDesc.SampleDesc = { 1,0 };
	resourceDesc.MipLevels = metadata.mipLevels;
	resourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	device->CreateCommittedResource(
		&heapProp,									//上記のプロップ
		D3D12_HEAP_FLAG_NONE,						//ヒープフラッグの属性
		&resourceDesc,								//上記のリソースデスク
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,	//リソースの状態
		nullptr,									//初期値(?)
		IID_PPV_ARGS(&texResource));				//ぶちこむ
	if (!texResource) {
		assert(false && "テクスチャリソースの作成ー失敗ー");
		return true;
	}

	//Mapみたいな(?)
    const DirectX::Image* image = scImage.GetImage(0, 0, 0);
	texResource->WriteToSubresource(
		0,					//サブリソース
		nullptr,			//書き込み範囲
		image->pixels,		//データ
		image->rowPitch,	//バイト数
		image->slicePitch);	//スライスのバイト数

	return false;
}

//シェーダーリソースビューの作成
void TextureData::SetSRV(ID3D12DescriptorHeap* heap, ID3D12Device* device)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	auto cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(texResource, &srvDesc, cpuHandle);
}