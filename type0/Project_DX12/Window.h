#pragma once

#include <Windows.h>
#include <utility>
#include <string>

#include <cassert>

//ウィンドウクラス
class Window
{
private:
	HWND handle{};	//ウィンドウハンドル

public:
	Window() = default;	//コンストラクタ
	~Window() = default;//デストラクタ

	//ウィンドウの生成
	bool Create(HINSTANCE,std::pair<UINT,UINT>, std::string);

	//HWNDの取得
	HWND GetHWND() const { return handle; }
};