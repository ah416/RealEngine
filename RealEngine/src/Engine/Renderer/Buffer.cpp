#include "realpch.h"

#include "Buffer.h"

#include "../OpenGL/OpenGLBuffer.h"

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
	return new OpenGLVertexBuffer(vertices, size);
}

VertexBuffer* VertexBuffer::Create(std::vector<glm::vec2>& uvs, uint32_t size)
{
	return new OpenGLVertexBuffer(uvs, size);
}

VertexBuffer* VertexBuffer::Create(std::vector<glm::vec3>& vertices, uint32_t size)
{
	return new OpenGLVertexBuffer(vertices, size);
}

VertexBuffer* VertexBuffer::Create(void* data, uint32_t size)
{
	return new OpenGLVertexBuffer(data, size);
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
{
	return new OpenGLIndexBuffer(indices, size);
}

IndexBuffer* IndexBuffer::Create(std::vector<uint32_t>& indices, uint32_t size)
{
	return new OpenGLIndexBuffer(indices, size);
}

IndexBuffer* IndexBuffer::Create(void* data, uint32_t size)
{
	return new OpenGLIndexBuffer(data, size);
}