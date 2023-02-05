#pragma once

#include "Application.h"

class Input
{
public:
	// Gets the status of the key, returns true if pressed
	static bool IsKeyPressed(int keycode);

	// Gets the status of the mouse button, returns true if pressed
	static bool IsMouseButtonPressed(int button);

	static std::pair<float, float> GetMousePosition();
};