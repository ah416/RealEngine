#pragma once

#include <sstream>
#include <string>
#include <functional>

enum class EventType
{
	None = 0,
	KeyPress, KeyRelease, KeyTyped,
	MouseButtonPress, MouseButtonRelease, MouseScrolled, MouseMoved,
	WindowClose, WindowResize
};

class Event
{
	friend class EventDispatcher;
public:
	virtual EventType GetType() const = 0;
	virtual std::string ToString() const { return "NotImplemented"; }

	bool Handled = false;
};

class EventDispatcher
{
	template<typename T>
	using EventFn = std::function<bool(T&)>;
public:
	EventDispatcher(Event& e) : m_Event(e) {}

	template<class T>
	bool Dispatch(EventFn<T> func)
	{
		if (m_Event.GetType() == T::GetStaticType())
		{
			m_Event.Handled = func(*(T*)&m_Event);
			return true;
		}
		return false;
	}
private:
	Event& m_Event;
};

class KeyEvent : public Event
{
public:
	inline int GetKeycode() { return m_KeyCode; }

protected:
	KeyEvent(int keycode) : m_KeyCode(keycode) {}

	int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int keycode, int repeat) : KeyEvent(keycode), m_Repeat(repeat) {}

	inline int GetRepeat() { return m_Repeat; }

	static EventType GetStaticType() { return EventType::KeyPress; }

	EventType GetType() const override { return GetStaticType(); }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " Repeat: " << m_Repeat;
		return ss.str();
	}

private:
	int m_Repeat;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

	static EventType GetStaticType() { return EventType::KeyRelease; }

	EventType GetType() const override { return EventType::KeyRelease; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyCode;
		return ss.str();
	}
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

	static EventType GetStaticType() { return EventType::KeyTyped; }

	EventType GetType() const override { return EventType::KeyTyped; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}
};

class MouseButtonEvent : public Event
{
public:
	inline int GetButtonCode() { return m_KeyCode; }

protected:
	MouseButtonEvent(int keycode) : m_KeyCode(keycode) {}

	int m_KeyCode;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int keycode, int repeat) : MouseButtonEvent(keycode), m_Repeat(repeat) {}

	static EventType GetStaticType() { return EventType::MouseButtonPress; }

	EventType GetType() const override { return GetStaticType(); }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_KeyCode << " Repeat: " << m_Repeat;
		return ss.str();
	}
private:
	int m_Repeat;
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int keycode) : MouseButtonEvent(keycode) {}

	static EventType GetStaticType() { return EventType::MouseButtonRelease; }

	EventType GetType() const override { return GetStaticType(); }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_KeyCode;
		return ss.str();
	}
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(double xOffset, double yOffset) : xOffset(xOffset), yOffset(yOffset)
	{}

	static EventType GetStaticType() { return EventType::MouseScrolled; }

	EventType GetType() const override { return GetStaticType(); }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << xOffset << " " << yOffset;
		return ss.str();
	}

	inline double GetXOffset() const { return xOffset; }
	inline double GetYOffset() const { return yOffset; }
private:
	double xOffset, yOffset;
};

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float xPos, float yPos) : m_MouseX(xPos), m_MouseY(yPos) {}

	static EventType GetStaticType() { return EventType::MouseMoved; }

	EventType GetType() const override { return GetStaticType(); }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << " " << m_MouseY;
		return ss.str();
	}

	inline float GetMouseX() const { return m_MouseX; }
	inline float GetMouseY() const { return m_MouseY; }

private:
	float m_MouseX, m_MouseY;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	static EventType GetStaticType() { return EventType::WindowClose; }

	EventType GetType() const override { return GetStaticType(); }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowCloseEvent";
		return ss.str();
	}
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}

	static EventType GetStaticType() { return EventType::WindowResize; }

	EventType GetType() const override { return GetStaticType(); }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_Width << " " << m_Height;
		return ss.str();
	}

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

private:
	int m_Width, m_Height;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.ToString();
}
