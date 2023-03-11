#include "realpch.h"

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

OpenGLContext::OpenGLContext(void* window)
{
	m_GLFWWindow = (GLFWwindow*)window;
	if (!m_GLFWWindow)
		REAL_CRITICAL("OpenGLContext: Window passed into contructor is null!");
}

void OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_GLFWWindow);

	// Current glfw window must be the current context
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!status)
		REAL_CRITICAL("Glad init error!");

	REAL_INFO("GPU Vendor: {}", (const char*)glGetString(GL_VENDOR));
	REAL_INFO("GPU: {}", (const char*)glGetString(GL_RENDERER));
	REAL_INFO("OpenGL and GPU Driver Version: {}", (const char*)glGetString(GL_VERSION));
}

void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_GLFWWindow);
}

void OpenGLContext::VSync(bool enable)
{
	glfwSwapInterval(enable);
}

void OpenGLContext::EnableDepth(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	m_3D = enable;
}