#pragma once

#include "Engine/Core/Window.h"

#include "Engine/Renderer/GraphicsContext.h"

struct GLFWwindow;

class WindowsWindow : public Window
{
public:
	WindowsWindow(const std::string& title, uint32_t width, uint32_t height, const char* icon_path);

	virtual ~WindowsWindow();

	virtual void Init() override;

	virtual void OnUpdate() override;

	virtual void SetCallback(const EventFn& callback) override { m_Data.EventCallback = callback; }

	virtual void SetVSync(bool enable) override { m_Context->VSync(enable); m_Data.VSync = enable; }

	virtual bool IsVSync() const override { return m_Data.VSync; }

	virtual unsigned int GetWidth() const override { return m_Data.Width; }

	virtual unsigned int GetHeight() const override { return m_Data.Height; }

	virtual void* GetNativeWindow() const override { return m_GLFWWindow; }

	virtual GraphicsContext* GetGraphicsContext() const override { return m_Context; }
private:
	GLFWwindow* m_GLFWWindow;
	GraphicsContext* m_Context;
	WindowData m_Data;
	const char* m_IconPath;
};