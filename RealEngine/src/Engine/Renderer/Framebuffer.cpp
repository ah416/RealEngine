#include "realpch.h"

#include "Framebuffer.h"
#include "OpenGL/OpenGLFramebuffer.h"

Framebuffer* Framebuffer::Create(int width, int height)
{
	return new OpenGLFramebuffer(width, height);
}