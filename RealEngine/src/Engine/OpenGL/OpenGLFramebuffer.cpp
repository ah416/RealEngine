#include "realpch.h"

#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

OpenGLFramebuffer::OpenGLFramebuffer(const int width, const int height) : m_Width(width), m_Height(height)
{
	Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
	glDeleteFramebuffers(1, &m_FramebufferID);
	glDeleteTextures(1, &m_RenderBufferID);
	glDeleteTextures(1, &m_TextureID);
}

void OpenGLFramebuffer::Invalidate()
{
	PROFILE_FUNCTION();

	if (m_FramebufferID)
	{
		glDeleteFramebuffers(1, &m_FramebufferID);
		glDeleteTextures(1, &m_RenderBufferID);
		glDeleteTextures(1, &m_TextureID);
	}

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &m_FramebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

	m_ColorTexture.reset(RenderTexture::Create(m_Width, m_Height));
	m_ColorTexture->Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->GetRendererID(), 0);

	// glGenTextures(1, &m_TextureID);
	// glBindTexture(GL_TEXTURE_2D, m_TextureID);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// if (m_FramebufferData)
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_FramebufferData);
	// else
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// // Set texture as our color attachment #0
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

	glGenTextures(1, &m_RenderBufferID);
	glBindTexture(GL_TEXTURE_2D, m_RenderBufferID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_RenderBufferID, 0);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		REAL_ERROR("Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	if (m_FramebufferData)
		m_FramebufferData = nullptr;
	Invalidate();
}

void OpenGLFramebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
	glViewport(0, 0, m_Width, m_Height);
}

void OpenGLFramebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int OpenGLFramebuffer::GetWidth() const
{
	return m_Width;
}

int OpenGLFramebuffer::GetHeight() const
{
	return m_Height;
}

Ref<RenderTexture> OpenGLFramebuffer::GetTexture() const
{
	return m_ColorTexture;
}

void OpenGLFramebuffer::SetData(void *data)
{
	m_FramebufferData = data;
	Invalidate();
}

uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID() const
{
	return m_ColorTexture->GetRendererID();
	//return m_TextureID;
}
