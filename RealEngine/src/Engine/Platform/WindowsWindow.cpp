#include "realpch.h"

#include "Engine/Platform/WindowsWindow.h"

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <stdexcept>

WindowsWindow::WindowsWindow(const std::string& title, uint32_t width, uint32_t height, const char* icon_path) : m_Context(nullptr), m_GLFWWindow(nullptr)
{
	m_Data.Title = title.c_str();
	m_Data.Width = width;
	m_Data.Height = height;

	m_IconPath = icon_path;
}

WindowsWindow::~WindowsWindow()
{
	delete m_Context;

	glfwDestroyWindow(m_GLFWWindow);
	glfwTerminate();
}

void WindowsWindow::Init()
{
	int success = glfwInit();
	REAL_ASSERT(success, "GLFW Init fail");

	/* glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); DON'T call this unless we use vulkan */

	GLFWwindow* window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title, nullptr, nullptr);
	m_GLFWWindow = window;

	m_Context = GraphicsContext::Create((void*)m_GLFWWindow);
	m_Context->Init();
	m_Context->EnableDepth(true);

	glfwSetWindowUserPointer(m_GLFWWindow, &m_Data);

	glfwSetErrorCallback([](int error_code, const char* description)
		{
			REAL_ERROR("GLFW Error #{}: {}", error_code, description);
		});

	glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	switch (action)
	{
	case GLFW_PRESS:
	{
		KeyPressedEvent e(key, 0);
		data.EventCallback(e);
		break;
	}
	case GLFW_RELEASE:
	{
		KeyReleasedEvent e(key);
		data.EventCallback(e);
		break;
	}
	case GLFW_REPEAT:
	{
		KeyPressedEvent e(key, 1);
		data.EventCallback(e);
		break;
	}
	}
		});
	glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			data.EventCallback(e);
		});

	glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent e(width, height);
			data.EventCallback(e);
		});

	glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	switch (action)
	{
	case GLFW_PRESS:
	{
		MouseButtonPressedEvent e(button, 0);
		data.EventCallback(e);
		break;
	}
	case GLFW_RELEASE:
	{
		MouseButtonReleasedEvent e(button);
		data.EventCallback(e);
		break;
	}
	case GLFW_REPEAT:
	{
		MouseButtonPressedEvent e(button, 1);
		data.EventCallback(e);
		break;
	}
	}
		});

	glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	MouseScrolledEvent e((float)xoffset, (float)yoffset);
	data.EventCallback(e);
		});
	glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	MouseMovedEvent e((float)xPos, (float)yPos);
	data.EventCallback(e);
		});

	glfwSetCharCallback(m_GLFWWindow, [](GLFWwindow* window, uint32_t codepoint)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	KeyTypedEvent e(codepoint);
	data.EventCallback(e);
		});

	// Code to add a WindowsWindow icon
	if (strcmp(m_IconPath, "") == 0)
		return;

	GLFWimage icon{};
	int channels;
	icon.pixels = stbi_load(m_IconPath, &icon.width, &icon.height, &channels, 4);
	if (icon.pixels)
		glfwSetWindowIcon(window, 1, &icon);
	stbi_image_free(icon.pixels);
}

void WindowsWindow::OnUpdate()
{
	glfwPollEvents();
	m_Context->SwapBuffers();
}

Window* Window::Create(const std::string& title, uint32_t width, uint32_t height, const char* icon_path)
{
	return new WindowsWindow(title, width, height, icon_path);
}