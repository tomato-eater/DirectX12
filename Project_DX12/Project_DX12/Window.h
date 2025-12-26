#pragma once

#include <Windows.h>
#include <string>
using namespace std;
#include <cassert>

#include "Input.h"

class Window
{
private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND handle{};

public:
	Window() = default;
	~Window() = default;

	bool Create(HINSTANCE instance, string name, int wi, int hi);

	HWND GetHandle() { return handle; }
};

