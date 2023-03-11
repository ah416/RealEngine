#pragma once

#include "Engine/Renderer/Buffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	OpenGLVertexBuffer(std::vector<glm::vec2>& uvs, uint32_t size);
	OpenGLVertexBuffer(std::vector<glm::vec3>& vertices, uint32_t size);
	OpenGLVertexBuffer(void* data, uint32_t size);

	virtual ~OpenGLVertexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual const BufferLayout& GetLayout() const override { return m_Layout; }
	virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

private:
	uint32_t m_RendererID;
	BufferLayout m_Layout;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
	OpenGLIndexBuffer(std::vector<uint32_t>& indices, uint32_t count);
	OpenGLIndexBuffer(void* data, uint32_t size);
	virtual ~OpenGLIndexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual uint32_t GetCount() const { return m_Count; }

private:
	uint32_t m_Count;
	uint32_t m_RendererID;
};