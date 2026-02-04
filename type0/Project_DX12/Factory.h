#pragma once
#include <cassert>

#include <d3d12.h>
#include <dxgi1_6.h>

//ファクトリークラス
class Factory
{
private:
	IDXGIFactory6* factory;	//ファクトリーインターフェース

public:
	Factory() = default;//コンストラクタ
	~Factory();			//デストラクタ

	//ファクトリーの作成
	bool Create();

	//ファクトリーインターフェースの取得
	IDXGIFactory6* Get() const { return factory; }
};

