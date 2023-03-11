#pragma once

#include <RealEngine.h>

#include <stb_image.h>

class Convolution {
public:
	Convolution() {
		ReloadShader();
	}

	bool SetShaderPath(const std::string& filepath) {
		m_ShaderPath = filepath;
		return ReloadShader();
	}

	bool ReloadShader() {
		auto source = Shader::ReadShader(m_ShaderPath);
		if (source.empty()) {
			return false;
		}
		m_ConvolutionShader.reset(ComputeShader::Create(source));
		m_ConvolutionShader->Bind();
		m_ConvolutionShader->SetMat3("u_Gx", glm::mat3(1.0));
		m_ConvolutionShader->SetMat3("u_Gy", glm::mat3(1.0));
		m_ConvolutionShader->Unbind();
		return true;
	}

	bool SetImage(const std::string& filepath) {
		m_CurrentImage = filepath;
		return ResetImage();
	}

	bool ResetImage() {
		int w, h, c;
		stbi_set_flip_vertically_on_load(1);
		uint8_t* buf = stbi_load(m_CurrentImage.c_str(), &w, &h, &c, 4);
		if (!buf)
			return false;
		float* float_buf = (float*)malloc(sizeof(float) * w * h * 4);

		for (int i = 0; i < w * h * 4; i++)
			float_buf[i] = static_cast<float>(buf[i] / 255.0);

		m_ConvolveTex.reset(RenderTexture::Create(w, h, float_buf));
		m_TempTex.reset(RenderTexture::Create(w, h, float_buf));
		m_OriginalTex.reset(RenderTexture::Create(w, h, float_buf));
		m_SwapTextures = false;

		free(buf);
		free(float_buf);

		return true;
	}

	void Compute() {
		glm::mat3 u_Gx = { 1, 0, -1,
								   2, 0, -2,
								   1, 0, -1 };

		glm::mat3 u_Gy = { 1, 2, 1,
						   0, 0, 0,
						  -1, -2, -1 };

		if (!m_SwapTextures) {
			m_TempTex->Bind();
			m_ConvolveTex->Bind(1);
			m_SwapTextures = true;
		}
		else {
			m_ConvolveTex->Bind();
			m_TempTex->Bind(1);
			m_SwapTextures = false;
		}
		m_ConvolutionShader->Bind();
		m_ConvolutionShader->SetMat3("u_Gx", u_Gx);
		m_ConvolutionShader->SetMat3("u_Gy", u_Gy);
		m_ConvolutionShader->Dispatch(m_ConvolveTex->GetWidth() / 8, m_ConvolveTex->GetHeight() / 4, 1);
		m_ConvolutionShader->Unbind();
		m_ConvolveTex->Unbind();
		m_TempTex->Unbind();
	}

	void RenderImGui() {
		ImGui::Begin("Convolution", NULL, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::Button("Compute")) {
				this->Compute();
			}
			if (ImGui::BeginMenu("Options")) {
				if (ImGui::MenuItem("Reset Image")) {
					this->ResetImage();
				}
				if (ImGui::MenuItem("Load New Image")) {
					auto path = Filesystem::OpenFileDialog().string();
					if (!path.empty())
						this->SetImage(path);
				}
				if (ImGui::MenuItem("Reload Shader")) {
					ReloadShader();
					ResetImage();
				}
				if (ImGui::MenuItem("Load New Shader")) {
					auto path = Filesystem::OpenFileDialog().string();
					if (!path.empty()) {
						SetShaderPath(path);
						ResetImage();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::Image(reinterpret_cast<void*>((uint64_t)m_OriginalTex->GetRendererID()), ImVec2{ (float)m_OriginalTex->GetWidth(), (float)m_OriginalTex->GetHeight() }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::SameLine();
		if (!m_SwapTextures)
		{
			ImGui::Image(reinterpret_cast<void*>((uint64_t)m_ConvolveTex->GetRendererID()), ImVec2{ (float)m_ConvolveTex->GetWidth(), (float)m_ConvolveTex->GetHeight() }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
		else
		{
			ImGui::Image(reinterpret_cast<void*>((uint64_t)m_TempTex->GetRendererID()), ImVec2{ (float)m_TempTex->GetWidth(), (float)m_TempTex->GetHeight() }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
		ImGui::End();
	}
private:
	bool m_SwapTextures = false;
	std::string m_CurrentImage = "";

	std::string m_ShaderPath = "../RealEngine/Resources/Shaders/Convolution.glsl";
	Ref<ComputeShader> m_ConvolutionShader;
	Ref<RenderTexture> m_OriginalTex;
	Ref<RenderTexture> m_ConvolveTex;
	Ref<RenderTexture> m_TempTex;
};