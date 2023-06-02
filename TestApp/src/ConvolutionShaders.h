#pragma once

#include <RealEngine.h>

#include <stb_image.h>

class ConvolutionShaders {
public:
	ConvolutionShaders() {
		ReloadShader();
		for (int i = 0; i < 3; i++) {
			m_Kernel1[i] = glm::vec3(0.0f);
			m_Kernel2[i] = glm::vec3(0.0f);
		}
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
		float* float_buf = new float[w * h * 4];

		for (int i = 0; i < w * h * 4; i++)
			float_buf[i] = static_cast<float>(buf[i] / 255.0);

		m_ConvolveTex.reset(RenderTexture::Create(w, h, float_buf));
		m_TempTex.reset(RenderTexture::Create(w, h, float_buf));
		m_OriginalTex.reset(RenderTexture::Create(w, h, float_buf));
		m_SwapTextures = false;

		free(buf);
		delete[] float_buf;

		return true;
	}

	void SetKernel1(const glm::mat3& k) {
		m_Kernel1[0] = k[0];
		m_Kernel1[1] = k[1];
		m_Kernel1[2] = k[2];
	}

	void SetKernel2(const glm::mat3& k) {
		m_Kernel2[0] = k[0];
		m_Kernel2[1] = k[1];
		m_Kernel2[2] = k[2];
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
		glm::mat3 kernel1 = { m_Kernel1[0], m_Kernel1[1], m_Kernel1[2] };
		glm::mat3 kernel2 = { m_Kernel2[0], m_Kernel2[1], m_Kernel2[2] };
		m_ConvolutionShader->SetMat3("u_Gx", kernel1);
		m_ConvolutionShader->SetMat3("u_Gy", kernel2);
		m_ConvolutionShader->Dispatch(m_ConvolveTex->GetWidth() / 4, m_ConvolveTex->GetHeight() / 4, 1);
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
				if (ImGui::BeginMenu("Change Kernels")) {
					for (int i = 0; i < 3; i++) {
						char buf[256];
						snprintf(buf, 256, "kernel%i", i);
						ImGui::DragFloat3(buf, glm::value_ptr(m_Kernel1[i]), 0.01f, -2.0f, 2.0f);
					}
					ImGui::EndMenu();
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

	std::string m_ShaderPath = "../RealEngine/Resources/Shaders/PartialDerivatives.glsl";
	Ref<ComputeShader> m_ConvolutionShader;
	Ref<RenderTexture> m_OriginalTex;
	Ref<RenderTexture> m_ConvolveTex;
	Ref<RenderTexture> m_TempTex;

	glm::vec3 m_Kernel1[3];
	glm::vec3 m_Kernel2[3];
};