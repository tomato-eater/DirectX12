#pragma once

#include <utility>

//ゲームオブジェクトクラス
class ObjManager
{
private:
	//コンストラクタ　デストラクタ
	ObjManager() = default;
	~ObjManager() = default;

	uint64_t count{};	//オブジェクト識別番号

public:
	//インスタンス　取得　参照
	static ObjManager& Ins() {
		static ObjManager ins;
		return ins;
	}

	//オブジェクトの作成
	void Create(std::pair<float, float>);
};

