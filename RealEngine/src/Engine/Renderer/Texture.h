#pragma once

#include <string>

class Texture2D
{
public:
	virtual ~Texture2D() {}

	virtual void Bind(uint32_t slot = 0) const = 0;
	virtual void Unbind() const = 0;

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	virtual uint32_t GetRendererID() const = 0;

	static Texture2D* Create();
	static Texture2D* Create(const std::string& path);
	static Texture2D* Create(const int width, const int height, const uint8_t* data);
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
};