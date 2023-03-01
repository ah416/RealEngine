#include <RealEngine.h>
#include "Engine/ImGuiWindows/DebugWindow.h"

#include <stb_image.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

class TestLayer : public Layer
{
public:
	TestLayer() : Layer("Test"), m_Camera(Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight()) {}

	virtual void OnAttach() override
	{
		PROFILE_FUNCTION();

		Renderer::LoadShaders();

		m_Framebuffer.reset(Framebuffer::Create(Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight()));

		m_TestMesh.reset(new Mesh("../RealEngine/objects/Puro V2.fbx"));
		m_TestMesh->MeshMaterial->DiffuseTex.reset(Texture2D::Create("../RealEngine/textures/AmongUsAlbedo.png"));
		m_TestMesh->MeshMaterial->NormalTex.reset(Texture2D::Create("../RealEngine/textures/AmongUsNormal.png"));

		std::string computeShader = Shader::ReadShader("../RealEngine/Resources/Shaders/compute.shader");
		m_Compute.reset(ComputeShader::Create(computeShader));

		m_Tex.reset(RenderTexture::Create(1024, 1024));
		m_Compute->SetTexture(m_Tex);

		std::string raytracing_shader = Shader::ReadShader("../RealEngine/Resources/Shaders/RayTracing.shader");
		m_RayTracer.reset(ComputeShader::Create(raytracing_shader));

		m_RayTexture.reset(RenderTexture::Create(1024, 1024));
		m_RayTracer->SetTexture(m_RayTexture);

		std::string convolution_shader = Shader::ReadShader("../RealEngine/Resources/Shaders/Convolution.shader");
		m_ConvolutionShader.reset(ComputeShader::Create(convolution_shader));

		int w, h, c;
		uint8_t* buf = stbi_load("../RealEngine/textures/yote.png", &w, &h, &c, 4);
		float* float_buf = (float*)malloc(sizeof(float) * w * h * 4);

		for (int i = 0; i < w * h * 4; i++)
			float_buf[i] = static_cast<float>(buf[i] / 255.0);

		m_ConvolveTex.reset(RenderTexture::Create(w, h, float_buf));

		m_OriginalTex.reset(RenderTexture::Create(w, h, float_buf));

		m_TempTex.reset(RenderTexture::Create(w, h, float_buf));
		free(buf);
		free(float_buf);
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate(Timestep timestep) override
	{
		PROFILE_FUNCTION();

		// Always Clear and SetClearColor before rendering
		RenderCommand::Clear();
		RenderCommand::SetClearColor(m_ClearColor);

		m_RayTexture->Bind(1);
		m_RayTracer->Bind();
		m_RayTracer->Dispatch(1024 / 8, 1024 / 4, 1);
		m_RayTracer->Unbind();

		// Bind framebuffer before rendering
		m_Framebuffer->Bind();

		// Clear and set the clear color
		RenderCommand::Clear();
		RenderCommand::SetClearColor(m_ClearColor);

		// Give the renderer the camera position and view/transformation matrix
		Renderer::BeginScene(m_Camera);

		// Check for keypresses that affect camera TODO: should be changed to camera controller
		m_Camera.ProcessKeyboardInput(timestep);

		// Submit the mesh to the renderer
		// Renderer::Submit(m_TestMesh, glm::translate(glm::mat4(1.0), { 0.0, 0.0, 0.0 }));

		// Unbind the framebuffer
		m_Framebuffer->Unbind();

		Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		PROFILE_FUNCTION();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // make viewport the proper dimensions without padding
		ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;
		ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);

		ImGui::CaptureMouseFromApp(false);

		ImVec2 contentArea = ImGui::GetContentRegionAvail();
		if (contentArea.x != m_Framebuffer->GetWidth() || contentArea.y != m_Framebuffer->GetHeight())
		{
			m_Framebuffer->Resize((uint32_t)contentArea.x, (uint32_t)contentArea.y);
		}
		if (contentArea.x != m_Camera.GetWindowWidth() || contentArea.y != m_Camera.GetWindowHeight())
		{
			m_Camera.SetWindowWidth((int)contentArea.x);
			m_Camera.SetWindowHeight((int)contentArea.y);
		}

		if (m_ShowCompute)
			ImGui::Image(reinterpret_cast<void*>(m_RayTexture->GetRendererID()), ImVec2{ 2048, 2048 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		else
			ImGui::Image(reinterpret_cast<void*>(m_Framebuffer->GetColorAttachmentRendererID()), ImVec2{ contentArea.x, contentArea.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }); // , ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::IsItemHovered())
			m_ViewportHovered = true;
		else
			m_ViewportHovered = false;

		ImGui::End();
		ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Left;
		ImGui::PopStyleVar();

		ImGui::Begin("Original Image", NULL, ImGuiWindowFlags_NoDocking);
		ImGui::Image(reinterpret_cast<void*>(m_OriginalTex->GetRendererID()), ImVec2{ 512, 512 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();

		ImGui::Begin("Convolved Image", NULL, ImGuiWindowFlags_NoDocking);
		if (!m_SwapTextures)
		{
			ImGui::Image(reinterpret_cast<void*>(m_ConvolveTex->GetRendererID()), ImVec2{ 512, 512 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::Text("Using m_ConvolveTex");
		}
		else
		{
			ImGui::Image(reinterpret_cast<void*>(m_TempTex->GetRendererID()), ImVec2{ 512, 512 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::Text("Using m_TempTex");
		}
		ImGui::End();

		ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoDocking);
		ImGui::DragFloat4("Clear Color", glm::value_ptr(m_ClearColor), 0.005, 0.0, 1.0);
		if (ImGui::TreeNode("Shaders"))
		{
			if (ImGui::Button("Convolve")) {
				float kernel[9] = { 1 / 9.0, 1 / 9.0, 1 / 9.0, // box blur
									1 / 9.0, 1 / 9.0, 1 / 9.0,
									1 / 9.0, 1 / 9.0, 1 / 9.0 };
				//float kernel[9] = { 0, -1, 0, // spatial highpass
				//					-1, 4, -1,
				//					0, -1, 0 };
				if (!m_SwapTextures) {
					m_TempTex->Bind();
					m_ConvolveTex->Bind(1);
					m_SwapTextures = true;
					REAL_INFO("Using m_TempTex as output!");
				}
				else {
					m_ConvolveTex->Bind();
					m_TempTex->Bind(1);
					m_SwapTextures = false;
					REAL_INFO("Using m_ConvolveTex as output!");
				}
				m_ConvolutionShader->Bind();
				m_ConvolutionShader->SetFloatArray("u_Kernel", 9, kernel);
				m_ConvolutionShader->Dispatch(1024 / 8, 1024 / 4, 1);
				m_ConvolutionShader->Unbind();
				m_OriginalTex->Unbind();
				m_ConvolveTex->Unbind();
				m_TempTex->Unbind();
			}
			if (ImGui::Button("Reset Convolution")) {

				int w, h, c;
				uint8_t* buf = stbi_load("../RealEngine/textures/yote.png", &w, &h, &c, 4);
				float* float_buf = (float*)malloc(sizeof(float) * w * h * 4);

				for (int i = 0; i < w * h * 4; i++)
					float_buf[i] = static_cast<float>(buf[i] / 255.0);

				m_ConvolveTex.reset(RenderTexture::Create(w, h, float_buf));
				m_TempTex.reset(RenderTexture::Create(w, h, float_buf));
				m_Convolved = false;
				m_SwapTextures = false;

				free(buf);
				free(float_buf);
			}
			ImGui::TreePop();
		}
		ImGui::End();

		ImGui::Begin("Compute Shader / Framebuffer", NULL, ImGuiWindowFlags_NoDocking);
		if (ImGui::Button("ComputeShader/Framebuffer", { 180, 25 }))
		{
			m_ShowCompute ^= 1;
		}
		ImGui::End();
	}

	virtual void OnEvent(Event& e) override
	{
		EventType type = e.GetType();

		if (type == EventType::MouseMoved)
		{
			MouseMovedEvent mouse = *(MouseMovedEvent*)&e;
			if (m_ViewportHovered)
				m_Camera.ProcessMouseInput(mouse.GetMouseX(), mouse.GetMouseY());
		}

		if (type == EventType::MouseScrolled)
		{
			MouseScrolledEvent mouse = *(MouseScrolledEvent*)&e;
			if (mouse.GetYOffset() == -1)
				m_Camera.SetMovementSpeed(m_Camera.GetMovementSpeed() * 0.9f);
			else
				m_Camera.SetMovementSpeed(m_Camera.GetMovementSpeed() * 1.1f);
		}

		if (type == EventType::KeyPress)
		{
			KeyPressedEvent key = *(KeyPressedEvent*)&e;
			int keycode = key.GetKeycode();

			if (keycode == REAL_KEY_SPACE)
			{
				m_TestMesh->Scale(2.0f);
			}
		}

		if (type == EventType::WindowResize)
		{
			WindowResizeEvent resize = *(WindowResizeEvent*)&e;
			m_Camera.SetWindowWidth(resize.GetWidth());
			m_Camera.SetWindowHeight(resize.GetHeight());
			m_Camera.RecalculateViewProjection();

			m_Framebuffer->Resize(resize.GetWidth(), resize.GetHeight());
		}
	}

private:
	bool m_ViewportHovered = false;
	bool m_ShowCompute = false;

	bool m_FirstPass = true;

	// Camera for view and other matrices
	PerspectiveCamera m_Camera;

	// Framebuffer to render to
	Ref<Framebuffer> m_Framebuffer;

	// Mesh and associated textures
	Ref<Mesh> m_TestMesh;
	Ref<Texture2D> m_AmongTexture;
	Ref<Texture2D> m_NormalTex;

	// Compute Shader related variables
	Ref<ComputeShader> m_Compute;
	Ref<RenderTexture> m_Tex;
	Ref<ComputeShader> m_RayTracer;
	Ref<RenderTexture> m_RayTexture;

	Ref<ComputeShader> m_ConvolutionShader;
	Ref<RenderTexture> m_OriginalTex;
	Ref<RenderTexture> m_ConvolveTex;
	Ref<RenderTexture> m_TempTex;
	bool m_Convolved = false;
	bool m_SwapTextures = false;

	glm::vec4 m_ClearColor = glm::vec4(0.1, 0.1, 0.1, 1.0);
};

class TestApp : public Application
{
public:
	TestApp() : Application(1920, 1080, "Test Engine", "../RealEngine/textures/yote.png")
	{
		Ref<DebugWindow> debugWindow;
		debugWindow.reset(new DebugWindow());
		AddGUIWindow(debugWindow);

		AddLayer(new TestLayer());
	}
};

Application* CreateApplication()
{
#ifdef REAL_LINUX
	// to match msvc launching TestApp in this directory
	// TODO: move this to not be a hardcoded path because this is stupid
	std::filesystem::current_path("/home/adamh/Dev/RealEngine/TestApp");
#endif
	return new TestApp();
}
