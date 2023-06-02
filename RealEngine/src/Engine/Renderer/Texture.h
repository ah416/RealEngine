#pragma once

#include <string>

enum class TextureFormat {
	None = 0x8058, // Default format is RGBA8
	RGBA8 = 0x8058,
	RGBA16F = 0x881A,
	RGBA32F = 0x8814
};

class Texture2D
{
public:
	virtual ~Texture2D() {}

	virtual void Bind(uint32_t slot = 0) const = 0;
	virtual void Unbind() const = 0;

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	virtual uint32_t GetRendererID() const = 0;

	static Texture2D* Create(TextureFormat format = TextureFormat::RGBA8);
	static Texture2D* Create(const std::string& path, TextureFormat format = TextureFormat::RGBA8);
	static Texture2D* Create(const int width, const int height, const uint8_t* data, TextureFormat format = TextureFormat::RGBA8);
};

class RenderTexture
{
public:
	virtual ~RenderTexture() {}

	virtual void Bind(uint32_t slot = 0) const = 0;
	virtual void Unbind() const = 0;

	virtual uint32_t GetRendererID() const = 0;
	virtual void GetData(float* data) const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	static RenderTexture* Create(const int width, const int height);
	static RenderTexture* Create(const int width, const int height, const void* data);
};