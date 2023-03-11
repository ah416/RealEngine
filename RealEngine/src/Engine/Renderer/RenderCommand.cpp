#include "realpch.h"

#include "RenderCommand.h"

#include "Engine/Platform/OpenGL/OpenGLRendererAPI.h"

RendererAPI* RenderCommand::s_API = new OpenGLRendererAPI;