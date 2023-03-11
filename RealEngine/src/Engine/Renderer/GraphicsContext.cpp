#include "realpch.h"

#include "Engine/Renderer/GraphicsContext.h"

#include "Engine/Platform/OpenGL/OpenGLContext.h"

struct GLFWwindow;

GraphicsContext* GraphicsContext::Create(void* window)
{
    return new OpenGLContext(window);
}
