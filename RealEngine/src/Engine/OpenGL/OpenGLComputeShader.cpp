#include "realpch.h"

#include "OpenGLComputeShader.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

OpenGLComputeShader::OpenGLComputeShader(const std::string& shader) : m_RendererID(0), m_RenderTexture(0)
{
	GLuint compute = glCreateShader(GL_COMPUTE_SHADER);

	const GLchar* source = shader.c_str();
	glShaderSource(compute, 1, &source, NULL);
	glCompileShader(compute);

	// check for compilation errors
	GLint isCompiled = 0;
	glGetShaderiv(compute, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(compute, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(compute, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(compute);

		// Log the data from the infoLog
		REAL_ERROR("Compute shader compilation error: {0}", infoLog.data());

		// Maybe add other checking??
		return;
	}

	m_RendererID = glCreateProgram();
	GLuint program = m_RendererID;
	glAttachShader(program, compute);
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);

		// Don't leak shaders either.
		glDeleteShader(compute);

		// Log the error
		REAL_ERROR("Shader link failure! {0}", infoLog.data());

		// Add other error checking??
		return;
	}

	// Detach the shaders
	glDetachShader(program, compute);
}

OpenGLComputeShader::~OpenGLComputeShader()
{
	glDeleteProgram(m_RendererID);
}

void OpenGLComputeShader::Bind() const
{
	glUseProgram(m_RendererID);
}

void OpenGLComputeShader::Unbind() const
{
	glUseProgram(0);
}

void OpenGLComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z)
{
	glUseProgram(m_RendererID);
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	//glUseProgram(0);
}

/*
void OpenGLComputeShader::CreateTexture(uint32_t width, uint32_t height)
{
	glGenTextures(1, &m_RenderTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RenderTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, width, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, m_RenderTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}
*/

void OpenGLComputeShader::SetInt(const std::string& name, const int data)
{
	glUniform1i(GetUniformLocation(name), data);
}

void OpenGLComputeShader::SetUInt(const std::string& name, const unsigned data)
{
	glUniform1ui(GetUniformLocation(name), data);
}

void OpenGLComputeShader::SetFloat(const std::string& name, const float data)
{
	glUniform1f(GetUniformLocation(name), data);
}

uint32_t OpenGLComputeShader::GetTextureID() const
{
	return m_RenderTexture->GetRendererID();
}

Ref<RenderTexture> OpenGLComputeShader::GetTexture() const
{
	return m_RenderTexture;
}

int OpenGLComputeShader::GetUniformLocation(const std::string& name)
{
	PROFILE_FUNCTION();

	auto search = m_UniformLocationCache.find(name);
	if (search != m_UniformLocationCache.end())
		return search->second;

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		REAL_ERROR("Warning: uniform '{0}' could not be found!", name);

	m_UniformLocationCache[name] = location;
	return 0;
}
