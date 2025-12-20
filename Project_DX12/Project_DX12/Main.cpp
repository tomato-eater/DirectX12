#include "Window.h"


class Operations
{
private:
	pair<int, int> size = { 800,600 };
	string name = "error";

	Window wind{};


public:
	Operations() = default;
	~Operations() = default;

	bool Initialize(HINSTANCE instance)
	{
		if (wind.Create(instance, name, size.first, size.second))			return true;

	}

	void Loop()
	{
		MSG msg{};
		while (GetMessage(&msg, nullptr, 0, 0))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int nCmdShow)
{
	Operations op;

	if (op.Initialize(instance))
		return 0;

	op.Loop();

	return 0;
}