#pragma once

#include "Engine/Core/Event.h"

#include "Engine/Renderer/GraphicsContext.h"

#include <functional>

using EventFn = std::function<void(Event&)>;

class Window
{
public:
	Window() = default;
	virtual ~Window() {};

	// Initializes a new window with specified width, height, and titlebar
	virtual void Init() = 0;

	// Updates the window and glfw window
	virtual void OnUpdate() = 0;

	// Sets the event callback, for receiving all events from the glfw window
	virtual void SetCallback(const EventFn& callback) = 0;

	virtual void SetVSync(bool enable) = 0;
	virtual bool IsVSync() const = 0;

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	// returns either GLFWwindow* or hwnd
	virtual void* GetNativeWindow() const = 0;

	virtual GraphicsContext* GetGraphicsContext() const = 0;

	static Window* Create(const std::string& title, uint32_t width, uint32_t height, const char* icon_path);
protected:
	struct WindowData
	{
		const char* Title;
		unsigned int Width, Height;
		bool VSync;

		EventFn EventCallback;
	};
};