#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/Framebuffer.h"

class OpenGLFramebuffer : public Framebuffer
{
public:
	OpenGLFramebuffer(const int width, const int height);
	virtual ~OpenGLFramebuffer();

	virtual void Invalidate() override;

	virtual void Resize(uint32_t width, uint32_t height);

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual int GetWidth() const override;
	virtual int GetHeight() const override;

	virtual void SetData(void* data) override;

	virtual uint32_t GetColorAttachmentRendererID() const override;
private:
	int m_Width, m_Height;
	void* m_FramebufferData = nullptr;
	uint32_t m_FramebufferID;
	uint32_t m_TextureID;
	uint32_t m_RenderBufferID;
};
