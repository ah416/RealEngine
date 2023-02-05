#pragma once

#include "Engine/Core/Application.h"
#include "Engine/ImGuiWindows/EngineImGuiWindow.h"
#include "Engine/Timestep.h"
#include "Engine/Instrumentor.h"
#include "Engine/Renderer/Renderer.h"

#include "imgui.h"

#include <vector>

class DebugWindow : public EngineImGuiWindow
{
public:
	DebugWindow() : m_Frames(0), m_DisplayFPS(0), m_LastTime(0), m_LastTimeMicro(0) {}

	virtual void OnImGuiRender() override
	{
		PROFILE_FUNCTION();

		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		uint64_t micro = ((time.QuadPart - m_LastTimeMicro) * 1000000) / freq.QuadPart;
		m_LastTimeMicro = time.QuadPart;

		time.QuadPart = time.QuadPart / freq.QuadPart;
		m_Frames++;

		if ((time.QuadPart - m_LastTime) >= 1.0f)
		{
			m_DisplayFPS = m_Frames;
			m_Frames = 0;
			m_LastTime = time.QuadPart;
		}

		ImGui::Begin("Debug");
		ImGui::Text("Current FPS: %f", m_DisplayFPS);
		ImGui::Text("Frametime: %f milliseconds", (double)micro * 0.001);
		ImGui::Text("VSync: %s", Application::Get().GetWindow()->IsVSync() ? "true" : "false");
		if (ImGui::Button("Enable/Disable VSync", ImVec2(150, 25)))
			Application::Get().GetWindow()->SetVSync(Application::Get().GetWindow()->IsVSync() ? false : true);

		if (ImGui::TreeNode("Render Data"))
		{
			ImGui::Text("Draw Calls: %u", Renderer::GetRenderData().DrawCalls);
			ImGui::Text("Vertex Count: %llu", Renderer::GetRenderData().Vertices);
			ImGui::Text("Index Count: %llu", Renderer::GetRenderData().Indices);

			ImGui::TreePop();
		}

#ifdef PROFILE_IMGUI
		if (ImGui::TreeNode("Profile Timings"))
		{
			std::unordered_map<std::string, int64_t>& map = Instrumentor::Get().GetProfileMap();
			for (auto const& [name, duration] : map)
			{
				char label[250];
				strcpy_s(label, "%.3fms ");
				strcat_s(label, name.c_str());
				ImGui::Text(label, duration * 0.001);
			}

			ImGui::TreePop();
		}
#endif
		ImGui::End();
	}
private:
	uint64_t m_LastTime;
	uint64_t m_LastTimeMicro;
	float m_Frames;
	float m_DisplayFPS;
};