#include "realpch.h"

#include "VertexArray.h"

#include "Engine/Platform/OpenGL/OpenGLVertexArray.h"

VertexArray* VertexArray::Create()
{
	return new OpenGLVertexArray();
}