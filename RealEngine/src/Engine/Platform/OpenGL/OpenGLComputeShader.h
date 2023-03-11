#pragma once

#include "Engine/Renderer/ComputeShader.h"
#include "Engine/Renderer/Texture.h"

class OpenGLComputeShader : public ComputeShader
{
public:
	OpenGLComputeShader(const std::string& shader);
	virtual ~OpenGLComputeShader();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void Dispatch(uint32_t x, uint32_t y, uint32_t z) override;

	virtual void SetInt(const std::string& name, const int data) override;
	virtual void SetIntArray(const std::string& name, const int count, const int* data) override;
	virtual void SetUInt(const std::string& name, const unsigned data) override;
	virtual void SetFloat(const std::string& name, const float data) override;
	virtual void SetFloatArray(const std::string& name, const int count, const float* data) override;
	virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override;
	virtual void SetTexture(Ref<RenderTexture> tex) override { m_RenderTexture = tex; };

	virtual uint32_t GetTextureID() const override;
	virtual Ref<RenderTexture> GetTexture() const override;

private:
	int GetUniformLocation(const std::string& name);

private:
	uint32_t m_RendererID;
	Ref<RenderTexture> m_RenderTexture;
	std::unordered_map<std::string, int> m_UniformLocationCache;
};