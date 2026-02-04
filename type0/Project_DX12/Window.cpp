#include "Window.h"

// ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:	// ウィンドウが閉じられた
		PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ウィンドウの生成
bool Window::Create(HINSTANCE instance, std::pair<UINT, UINT> size, std::string title)
{
	// ウィンドウクラスの登録
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = title.c_str();
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	RegisterClass(&wc);

	// ウィンドウの作成
	handle = CreateWindow(
		wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		size.first, size.second, nullptr, nullptr, wc.hInstance, nullptr);

	if (!handle)
	{
		assert(false && "ウィンドウの作成ー失敗ー");
		return false;
	}

	// ウィンドウの表示
	ShowWindow(handle, SW_SHOW);

	// No abnormalitys
	return false;
}