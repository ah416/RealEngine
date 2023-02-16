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
	DebugWindow() : m_Frames(0), m_DisplayFPS(0), m_LastTime(0), m_LastFrameTime(0) {}

	virtual void OnImGuiRender() override
	{
		PROFILE_FUNCTION();

		double time = ImGui::GetTime();
		m_Frames++;
		double frametime = time - m_LastTime;
		m_LastTime = time;
		if ((time - m_LastFrameTime) >= 1.0f)
		{
			m_DisplayFPS = m_Frames;
			m_Frames = 0;
			m_LastFrameTime = time;
		}

		ImGui::Begin("Debug");
		ImGui::Text("Current FPS: %f", m_DisplayFPS);
		ImGui::Text("Frametime: %f milliseconds", frametime * 1000);
		ImGui::Text("VSync: %s", Application::Get().GetWindow()->IsVSync() ? "true" : "false");
		if (ImGui::Button("Enable/Disable VSync", ImVec2(150, 25)))
			Application::Get().GetWindow()->SetVSync(Application::Get().GetWindow()->IsVSync() ? false : true);

		if (ImGui::TreeNode("Render Data"))
		{
			ImGui::Text("Draw Calls: %u", Renderer::GetRenderData().DrawCalls);
			ImGui::Text("Vertex Count: %lu", Renderer::GetRenderData().Vertices);
			ImGui::Text("Index Count: %lu", Renderer::GetRenderData().Indices);

			ImGui::TreePop();
		}

#ifdef PROFILE_IMGUI
		if (ImGui::TreeNode("Profile Timings"))
		{
			std::unordered_map<std::string, int64_t> &map = Instrumentor::Get().GetProfileMap();
			for (auto const &[name, duration] : map)
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
	double m_LastTime;
	double m_LastFrameTime;
	float m_Frames;
	float m_DisplayFPS;
};