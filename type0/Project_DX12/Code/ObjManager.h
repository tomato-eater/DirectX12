#pragma once

//ゲームオブジェクトクラス
class ObjManager
{
private:
	//コンストラクタ　デストラクタ
	ObjManager() = default;
	~ObjManager() = default;

	//各ポリゴンの作成
	void CreatePoly();

	//各バッファの作成
	void CreateBuff();

public:
	//インスタンス　取得　参照
	static ObjManager& Ins() {
		static ObjManager ins;
		return ins;
	}

	//オブジェクトの作成
	void Create()
	{
		CreatePoly();	//ポリゴンの作成
		CreateBuff();	//バッファの作成　自分/敵　等
	}
};

