#pragma once

class RenderTexture;

class Framebuffer
{
public:
	virtual ~Framebuffer() {}

	virtual void Invalidate() = 0;

	virtual void Resize(uint32_t width, uint32_t height) = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	virtual Ref<RenderTexture> GetTexture() const = 0;

	virtual void SetData(void* data) = 0;

	virtual uint32_t GetColorAttachmentRendererID() const = 0;

	static Framebuffer* Create(int width, int height);
};