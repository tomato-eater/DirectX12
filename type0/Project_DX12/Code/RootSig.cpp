#include "RootSig.h"
#include <cassert>

#include "Device.h"

#pragma comment(lib, "d3d12.lib")

//ルートシグネチャーの生成
bool RootSig::Set(const D3D12_ROOT_SIGNATURE_DESC& rootDesc)
{
	//シリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> shigBlob{};
 	Microsoft::WRL::ComPtr<ID3DBlob> error{};

	if (D3D12SerializeRootSignature(
		&rootDesc,						//上記のデスク
		D3D_ROOT_SIGNATURE_VERSION_1_0,	//シグネチャーのバージョン
		&shigBlob,						//上記のシリアライズ
		&error)							//上記のエラー
		!= S_OK){
		if (error){
			const char* msg = static_cast<const char*>(error->GetBufferPointer());
			OutputDebugStringA(msg);
			OutputDebugStringA("\n");
		}
		assert(false && "ルートシグネチャーのシリアライズー失敗ー");
		return true;
	}

	//生成
	if (Device::Ins().Get()->CreateRootSignature(
		0,								//ノードマスク
		shigBlob->GetBufferPointer(),	//上記のシリアライズの場所を取得
		shigBlob->GetBufferSize(),		//上記のシリアライズのサイズに更新
		IID_PPV_ARGS(&rootShignature))	//ぶち込む
		!= S_OK){
		assert(false && "ルートシグネチャー生成ー失敗ー");
		return true;
	}

	return false;
}