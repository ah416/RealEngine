#pragma once

#include "Engine/Core/Core.h"

#include <glm/glm.hpp>

class Texture2D;
class RenderTexture;

class ComputeShader
{
public:
	virtual ~ComputeShader() {};

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	// This will NOT bind the shader
	virtual void Dispatch(uint32_t x, uint32_t y, uint32_t z) = 0;

	virtual void SetInt(const std::string& name, const int data) = 0;
	virtual void SetIntArray(const std::string& name, const int count, const int* data) = 0;
	virtual void SetUInt(const std::string& name, const unsigned data) = 0;
	virtual void SetFloat(const std::string& name, const float data) = 0;
	virtual void SetFloatArray(const std::string& name, const int count, const float* data) = 0;
	virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
	virtual void SetTexture(Ref<RenderTexture> tex) = 0;

	virtual uint32_t GetTextureID() const = 0;
	virtual Ref<RenderTexture> GetTexture() const = 0;

	static ComputeShader* Create(const std::string& shader);
};