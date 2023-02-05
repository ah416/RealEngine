#include <realpch.h>

#include "RendererAPI.h"
#include "Engine/OpenGL/OpenGLRendererAPI.h"

RendererAPI* RendererAPI::Create()
{
	return new OpenGLRendererAPI();
}
