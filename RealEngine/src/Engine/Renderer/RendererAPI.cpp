#include <realpch.h>

#include "RendererAPI.h"
#include "Engine/Platform/OpenGL/OpenGLRendererAPI.h"

RendererAPI* RendererAPI::Create()
{
	return new OpenGLRendererAPI();
}
