#pragma once

#include <Windows.h>
#include <utility>
#include <string>

//ウィンドウクラス
class Window
{
private:
	//コンストラクタ　デストラクタ
	Window() = default;	
	~Window() = default;

	HWND handle{};	//ウィンドウハンドル
	std::pair<UINT, UINT> size{};

public:
	//インスタンス　取得　参照
	static Window& Ins() {
		static Window win;
		return win;
	}

	//ウィンドウの生成
	bool Create(HINSTANCE,std::pair<UINT,UINT>, std::string);

	//HWNDの取得
	HWND Get() const { return handle; }

	//ウィンドウサイズの取得
	std::pair<UINT, UINT> Size() const { return size; }
};