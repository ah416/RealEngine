#include "realpch.h"

#include "VertexArray.h"

#include "Engine/OpenGL/OpenGLVertexArray.h"

VertexArray* VertexArray::Create()
{
	return new OpenGLVertexArray();
}