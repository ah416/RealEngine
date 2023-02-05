#pragma once

#include "Engine/Core/Core.h"

class CameraController
{
public:
	virtual ~CameraController() {}

	// Call methods from Input to determine the key presses
	virtual void ProcessKeyboardInput() = 0;

	// Call methods from Input to determine the button presses
	virtual void ProcessMouseInput(float xpos, float ypos) = 0;
};