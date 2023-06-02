#pragma once

#include "Engine/Renderer/Texture.h"

#include <string>

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(TextureFormat format = TextureFormat::RGBA8);
	OpenGLTexture2D(const std::string& path, TextureFormat format = TextureFormat::RGBA8);
	OpenGLTexture2D(const int width, const int height, const uint8_t* data, TextureFormat format = TextureFormat::RGBA8);

	virtual ~OpenGLTexture2D();

	virtual void Bind(uint32_t slot = 0) const;
	virtual void Unbind() const;

	virtual inline int GetWidth() const { return m_Width; }
	virtual inline int GetHeight() const { return m_Height; }

	virtual inline uint32_t GetRendererID() const { return m_RendererID; }

private:
	uint32_t m_RendererID;

	std::string m_FilePath;
	int m_Width, m_Height, m_Channels;
};

class OpenGLRenderTexture : public RenderTexture
{
public:
	OpenGLRenderTexture(const int width, const int height);
	OpenGLRenderTexture(const int width, const int height, const void* data);
	
	virtual ~OpenGLRenderTexture();

	virtual void Bind(uint32_t slot = 0) const;
	virtual void Unbind() const override;

	virtual uint32_t GetRendererID() const override { return m_RendererID; }
	virtual void GetData(float* data) const override;
	virtual int GetWidth() const override { return m_Width; }
	virtual int GetHeight() const override { return m_Height; }
private:
	uint32_t m_RendererID;
	int m_Width, m_Height;
};