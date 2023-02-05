#include "realpch.h"

#include "Engine/Renderer/GraphicsContext.h"

#include "Engine/OpenGL/OpenGLContext.h"

struct GLFWwindow;

GraphicsContext* GraphicsContext::Create(void* window)
{
    return new OpenGLContext((GLFWwindow*)window);
}
