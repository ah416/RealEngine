#include "realpch.h"

#include "Input.h"

#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(int keycode)
{
	PROFILE_FUNCTION();

	if (glfwGetKey((GLFWwindow*)Application::Get().GetWindow()->GetNativeWindow(), keycode) == GLFW_PRESS)
		return true;
	return false;
}

bool Input::IsMouseButtonPressed(int button)
{
	PROFILE_FUNCTION();

	if (glfwGetMouseButton((GLFWwindow*)Application::Get().GetWindow()->GetNativeWindow(), button) == GLFW_PRESS)
		return true;
	return false;
}

std::pair<float, float> Input::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos((GLFWwindow*)Application::Get().GetWindow()->GetNativeWindow(), &x, &y);
	return std::pair<float, float>((float)x, (float)y);
}
