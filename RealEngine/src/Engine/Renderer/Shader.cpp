#include "realpch.h"

#include "Shader.h"
#include <filesystem>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) : m_RendererID(0)
{
	PROFILE_FUNCTION();

	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = vertexSrc.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		// Use the infoLog as you see fit.
		REAL_ERROR("Vertex shader compilation error: {0}", infoLog.data());

		// In this simple program, we'll just leave
		return;
	}

	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = fragmentSrc.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		// Use the infoLog as you see fit.
		REAL_ERROR("Fragment shader compilation failed: {0}", infoLog.data());

		// In this simple program, we'll just leave
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_RendererID = glCreateProgram();
	GLuint program = m_RendererID;

	// Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link our program
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
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Use the infoLog as you see fit.
		REAL_ERROR("Shader link failure: {0}", infoLog.data());

		// In this simple program, we'll just leave
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

std::string Shader::ReadShader(const std::string& filename)
{
	PROFILE_FUNCTION();

	std::string line;
	std::ifstream fs;
	std::stringstream ss;

	fs.open(filename, std::ifstream::in);

	if (fs.is_open())
	{
		while (getline(fs, line))
		{
			ss << line << "\n";
		}
		fs.close();
		return ss.str();
	}
	else
	{
		REAL_ERROR("Shader::ReadShader({0}) failed!", filename);
		REAL_ERROR("Current path: {0}", std::filesystem::current_path());
		return std::string();
	}
}

int Shader::GetUniformLocation(const std::string& name)
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

void Shader::SetBool(const std::string& name, const bool value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::UploadUniformInt1(const std::string& name, const int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::UploadUniformFloat1(const std::string& name, const float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec)
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vec));
}

void Shader::UploadUniformMat3(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderLibrary::Add(Ref<Shader> shader)
{
}
