#pragma once

#include "Engine/Core/Core.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Shader
{
public:
	/* 
	* Creates and compiles a shader program from source code
	* \param vertexSrc : vertex shader source code
	* \param fragmentSrc: fragment shader source code
	*/
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void Bind() const;
	void Unbind() const;
	
	// Attempts to retrieve a uniform location from OpenGL, then caches it within an unordered map
	int GetUniformLocation(const std::string& name);

	void SetBool(const std::string& name, const bool value);

	void UploadUniformInt1(const std::string& name, const int value);

	void UploadUniformFloat1(const std::string& name, const float value);
	void UploadUniformFloat3(const std::string& name, const glm::vec3& vec);

	void UploadUniformMat3(const std::string& name, const glm::mat4& matrix);
	void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	static std::string ReadShader(const std::string& filename);

	inline uint32_t GetRendererID() const { return m_RendererID; }

private:
	std::unordered_map<std::string, int> m_UniformLocationCache;
	uint32_t m_RendererID;
};

class ShaderLibrary
{
public:
	void Add(Ref<Shader> shader);

private:
	std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};
