#pragma once

#include "Engine/Renderer/GraphicsContext.h"

#include <iostream>

struct GLFWwindow;

class OpenGLContext : public GraphicsContext
{
public:
	OpenGLContext(GLFWwindow* glfwWindow);

	virtual void Init() override;
	virtual void SwapBuffers() override;
	virtual void VSync(bool enable) override;
	virtual void EnableDepth(bool enable) override;
private:
	GLFWwindow* m_GLFWWindow;
	bool m_3D = false;
};