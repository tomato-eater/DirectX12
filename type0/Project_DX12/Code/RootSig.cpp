#include "RootSig.h"

//デストラクタ
RootSig::~RootSig()
{
	if (rootShignature)
	{
		rootShignature->Release();
		rootShignature = nullptr;
	}
}

//ルートシグネチャーの生成
bool RootSig::Create(ID3D12Device* device, const ScreenRoot& rootDesc)
{
	//シリアライズ
	ID3DBlob* shigBlob{};
	ID3DBlob* error{};

	if (D3D12SerializeRootSignature(
		&rootDesc.desc,						//上記のデスク
		D3D_ROOT_SIGNATURE_VERSION_1_0,	//シグネチャーのバージョン
		&shigBlob,						//上記のシリアライズ
		&error))						//上記のエラー
	{
		if (error){
			const char* msg = static_cast<const char*>(error->GetBufferPointer());
			OutputDebugStringA(msg);
			OutputDebugStringA("\n");
		}
		assert(false && "ルートシグネチャーのシリアライズー失敗ー");
		return true;
	}

	//生成
	device->CreateRootSignature(
		0,								//ノードマスク
		shigBlob->GetBufferPointer(),	//上記のシリアライズの場所を取得
		shigBlob->GetBufferSize(),		//上記のシリアライズのサイズに更新
		IID_PPV_ARGS(&rootShignature));	//ぶち込む
	if (!rootShignature) {
		assert(false && "ルートシグネチャー生成ー失敗ー");
		return true;
	}

	return false;
}