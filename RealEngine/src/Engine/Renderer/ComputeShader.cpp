#include "realpch.h"

#include "ComputeShader.h"

#include "Engine/Platform/OpenGL/OpenGLComputeShader.h"

ComputeShader* ComputeShader::Create(const std::string& shader)
{
	return new OpenGLComputeShader(shader);
}
