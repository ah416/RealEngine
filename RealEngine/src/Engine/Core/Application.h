#pragma once

#include "Core.h"

#include "Engine/Core/Window.h"
#include "Engine/Core/Event.h"
#include "Engine/Timestep.h"
#include "Engine/Core/ImGuiLayer.h"
#include "Engine/ImGuiWindows/EngineImGuiWindow.h"
#include "Engine/Core/Layer.h"

class Application
{
public:
	/* 
	* Creates the main application, only one may exist
	* \param width: width of the window
	* \param height: height of the window
	* \param title: the title bar of the window
	*/
	Application(int width = 1920, int height = 1080, const char* title = "Engine", const char* icon_image = "");
	~Application() { }

	// The main run loop, calculates the frametime and updates all layers
	void Run();

	// Handles events passed to us through our set glfwCallbacks
	void OnEvent(Event& e);

	// Adds a layer to a primitive "layerstack"
	void AddLayer(Layer* layer) { m_LayerStack.push_back(layer); }

	// Adds an ImGui window to be rendered
	void AddGUIWindow(std::shared_ptr<EngineImGuiWindow> window) { m_ImGuiWindows.push_back(window); }

	// Retrieves the current window
	std::shared_ptr<Window> GetWindow() const { return m_Window; }

	// Returns the last calculated frametime
	uint64_t GetFrametime() const { return m_LastFrameTime; }

	// Returns a reference to the static application instance
	static Application& Get() { return *s_Instance; }

	Application(const Application&) = delete;
	Application* operator=(const Application*) = delete;

private:
	// Called on a WindowCloseEvent being received
	bool OnWindowClose(WindowCloseEvent& e);

	// Called on a WindowResizeEvent being received
	bool OnWindowResize(WindowResizeEvent& e);

private:
	// The single application instance
	static Application* s_Instance;

	// Is the engine still running?
	bool m_Running = true;

	// The window
	std::shared_ptr<Window> m_Window;

	// The "layerstack", holds all layers
	std::vector<Layer*> m_LayerStack;

	// The ImGuiLayer instance, used to render ImGui
	ImGuiLayer m_ImGuiLayer;

	// All of the current prefab ImGuiWindows in use
	std::vector<std::shared_ptr<EngineImGuiWindow>> m_ImGuiWindows;

	// The last calculated frametime
	uint64_t m_LastFrameTime;
};
