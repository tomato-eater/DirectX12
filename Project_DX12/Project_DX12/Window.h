#pragma once


#include <Windows.h>
#include <string>
using namespace std;
#include <cassert>

class Window
{
private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND handle{};

public:
	Window() = default;
	~Window() = default;

	[[nodiscard]] bool Create(HINSTANCE instance, string name, int wi, int hi);
};

