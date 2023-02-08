#include "realpch.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Input.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Shapes/Cube.h"
#include "Engine/Renderer/Texture.h"

#include <GLFW/glfw3.h>

Application* Application::s_Instance = nullptr;

Application::Application(int width, int height, const char* title, const char* icon_image)
	: m_LastFrameTime(0)
{
	PROFILE_FUNCTION();

	REAL_ASSERT(!s_Instance, "Error! Application has already been created!");

	s_Instance = this;

	m_Window.reset(Window::Create(title, width, height, icon_image));
	m_Window->Init();

	m_Window->SetVSync(true);

	m_Window->SetCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

	m_ImGuiLayer.OnAttach();
}

void Application::Run()
{
#ifdef _WIN32
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
#endif // _WIN32

	for (Layer* layer : m_LayerStack)
	{
		layer->OnAttach();
	}

	while (m_Running)
	{
#ifdef _WIN32
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		Timestep timestep = ((time.QuadPart - m_LastFrameTime) * 1000000) / freq.QuadPart; /* 1,000,000 is the number to convert to microseconds (for... accuracy?) */
		m_LastFrameTime = time.QuadPart;
#else
		double time = glfwGetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;
#endif // _WIN32

		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate(timestep);
		}

		m_ImGuiLayer.Begin();
		for (auto& window : m_ImGuiWindows)
		{
			window->OnImGuiRender();
		}
		for (auto& layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}
		m_ImGuiLayer.End();

		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	PROFILE_FUNCTION();

	m_ImGuiLayer.OnEvent(e);

	if (e.Handled)
		return;

	for (Layer* layer : m_LayerStack)
	{
		layer->OnEvent(e);
	}

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
	dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	RenderCommand::ResizeViewport(0, 0, e.GetWidth(), e.GetHeight());
	return false;
}
