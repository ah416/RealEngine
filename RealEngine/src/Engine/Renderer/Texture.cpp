#include <realpch.h>

#include "Texture.h"
#include "Engine/OpenGL/OpenGLTexture.h"

//
// Texture2D
//

Texture2D* Texture2D::Create()
{
	return new OpenGLTexture2D();
}

Texture2D* Texture2D::Create(const std::string& path)
{
	return new OpenGLTexture2D(path);
}

Texture2D* Texture2D::Create(const int width, const int height, const uint8_t* data)
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
