#pragma once

#include <dinput.h>
#include "Window.h"

class Input
{
private:
	byte keyState[256] = {};

public:
	static Input& Instance()
	{
		static Input instan;
		return instan;
	}

	Input() = default;
	~Input() = default;

	bool GetKey(uint16_t key) { return keyState[key] & 0x80; }

	void KeyState(void* state) { memcpy(keyState, state, sizeof(keyState)); }
};

