#include <realpch.h>

#include "Texture.h"
#include "Engine/Platform/OpenGL/OpenGLTexture.h"

//
// Texture2D
//

Texture2D* Texture2D::Create(TextureFormat format)
{
	return new OpenGLTexture2D();
}

Texture2D* Texture2D::Create(const std::string& path, TextureFormat format)
{
	return new OpenGLTexture2D(path);
}

Texture2D* Texture2D::Create(const int width, const int height, const uint8_t* data, TextureFormat format)
{
	return new OpenGLTexture2D(width, height, data);
}

//
// RenderTexture
//

RenderTexture* RenderTexture::Create(const int width, const int height)
{
	return new OpenGLRenderTexture(width, height);
}

RenderTexture* RenderTexture::Create(const int width, const int height, const void* data)
{
	return new OpenGLRenderTexture(width, height, data);
}
