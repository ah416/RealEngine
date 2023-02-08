#include <RealEngine.h>
#include "HeightmapCreator/Heightmapper.h"
#include "Engine/ImGuiWindows/DebugWindow.h"

#include <glm/gtc/matrix_transform.hpp>

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
		m_RayTexture->Bind(1);
		m_RayTracer->SetTexture(m_RayTexture);
		m_RayTexture->Unbind();

		m_RayTexture->Bind(1);
		m_RayTracer->Bind();
		m_RayTracer->Dispatch(1024 / 8, 1024 / 4, 1);
		m_RayTracer->Unbind();

		float *pixel_buf = (float *)malloc(sizeof(float) * (1024 * 1024) * 4);
		uint8_t *conversion_buf = (uint8_t *)malloc(sizeof(uint8_t) * (1024 * 1024) * 4);
		m_RayTexture->GetData(pixel_buf);
		for (int i = 0; i < (1024 * 1024) * 4; i++)
			conversion_buf[i] = static_cast<uint8_t>(pixel_buf[i] * 255.0f);
		Heightmapper map(1024, 1024, 4);
		map.SetData(conversion_buf);
		map.Write("something"); // frees the data passed to it (on stbi_write_png) :3
		free(pixel_buf);
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate(Timestep timestep) override
	{
		PROFILE_FUNCTION();

		// Always Clear and SetClearColor before rendering
		RenderCommand::Clear();
		RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});

		// Bind compute shader, set uniform, and Dispatch
		m_Tex->Bind();
		m_Compute->Bind();
		m_Compute->SetUInt("RandomFromCPU", std::random_device{}());
		m_Compute->Dispatch(1024 / 8, 1024 / 4, 1);
		m_Compute->Unbind();

		m_RayTexture->Bind(1);
		m_RayTracer->Bind();
		m_RayTracer->Dispatch(1024 / 8, 1024 / 4, 1);
		m_RayTracer->Unbind();

		// Bind framebuffer before rendering
		m_Framebuffer->Bind();

		// Clear and set the clear color
		RenderCommand::Clear();
		RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});

		// Give the renderer the camera position and view/transformation matrix
		Renderer::BeginScene(m_Camera);

		// Check for keypresses that affect camera TODO: should be changed to camera controller
		m_Camera.ProcessKeyboardInput(timestep);

		// Submit the mesh to the renderer
		Renderer::Submit(m_TestMesh, glm::translate(glm::mat4(1.0), {0.0, 0.0, 0.0}));

		// Unbind the framebuffer
		m_Framebuffer->Unbind();

		Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		PROFILE_FUNCTION();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // make viewport the proper dimensions without padding
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
			ImGui::Image((void *)m_Compute->GetTextureID(), ImVec2{2048, 2048}, ImVec2{0, 1}, ImVec2{1, 0});
		else
			ImGui::Image((void *)m_Framebuffer->GetColorAttachmentRendererID(), ImVec2{contentArea.x, contentArea.y}, ImVec2{0, 1}, ImVec2{1, 0}); // , ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::IsItemHovered())
			m_ViewportHovered = true;
		else
			m_ViewportHovered = false;

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Compute Shader / Framebuffer", NULL, ImGuiWindowFlags_NoDocking);
		if (ImGui::Button("ComputeShader/Framebuffer", {180, 25}))
		{
			m_ShowCompute ^= 1;
		}
		ImGui::End();
	}

	virtual void OnEvent(Event &e) override
	{
		EventType type = e.GetType();

		if (type == EventType::MouseMoved)
		{
			MouseMovedEvent mouse = *(MouseMovedEvent *)&e;
			if (m_ViewportHovered)
				m_Camera.ProcessMouseInput(mouse.GetMouseX(), mouse.GetMouseY());
		}

		if (type == EventType::MouseScrolled)
		{
			MouseScrolledEvent mouse = *(MouseScrolledEvent *)&e;
			if (mouse.GetYOffset() == -1)
				m_Camera.SetMovementSpeed(m_Camera.GetMovementSpeed() * 0.9f);
			else
				m_Camera.SetMovementSpeed(m_Camera.GetMovementSpeed() * 1.1f);
		}

		if (type == EventType::KeyPress)
		{
			KeyPressedEvent key = *(KeyPressedEvent *)&e;
			int keycode = key.GetKeycode();

			if (keycode == REAL_KEY_SPACE)
			{
				m_TestMesh->Scale(2.0f);
			}
		}

		if (type == EventType::WindowResize)
		{
			WindowResizeEvent resize = *(WindowResizeEvent *)&e;
			m_Camera.SetWindowWidth(resize.GetWidth());
			m_Camera.SetWindowHeight(resize.GetHeight());
			m_Camera.RecalculateViewProjection();

			m_Framebuffer->Resize(resize.GetWidth(), resize.GetHeight());
		}
	}

private:
	bool m_ViewportHovered = false;
	bool m_ShowCompute = false;

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

Application *CreateApplication()
{
	std::filesystem::current_path("/home/adam/Dev/RealEngine/TestApp");
	::Logger::Get().Info("current_path: {}", std::filesystem::current_path());
	::Logger::Get().Info("exists?: {}", std::filesystem::exists("../RealEngine/src/Engine/OpenGL/ImGuiImpl/Fonts/CascadiaCode.ttf"));
	return new TestApp();
}