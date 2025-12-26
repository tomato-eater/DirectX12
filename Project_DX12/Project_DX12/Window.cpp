#include "Window.h"

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        //画面更新処理
        return 0;

    case WM_KEYDOWN:
        //キー入力処理
        return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool Window::Create(HINSTANCE instance, string name, int wi, int hi)
{
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = name.c_str();
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClass(&wc);

    handle = CreateWindow
    (
        wc.lpszClassName, wc.lpszClassName,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        wi, hi, nullptr, nullptr, instance, nullptr
    );

    if (!handle)
    {
        assert(false && "ウィンドウの作成ー失敗ー");
        return true;
    }

    ShowWindow(handle, SW_SHOW);
    UpdateWindow(handle);

    return false;
}