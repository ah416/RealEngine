#pragma once

#include "Engine/ImGuiWindows/EngineImGuiWindow.h"
#include "Engine/OpenGL/ImGuiImpl/ImGuiLayerOpenGL.h"

class ColorPickerWindow : public EngineImGuiWindow
{
public:
	ColorPickerWindow() {}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Color Picker");

		static float* button = new float[4];
		ImGui::ColorEdit4("Button", button);

		static float* buttonActive = new float[4];
		ImGui::ColorEdit4("Hovered Button", buttonActive);

		static float* background = new float[4];
		ImGui::ColorEdit4("Background", background);

		static float* titleBackground = new float[4];
		ImGui::ColorEdit4("Title Background (inactive)", titleBackground);

		static float* titleBgActive = new float[4];
		ImGui::ColorEdit4("Title Background (active)", titleBgActive);

		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Button] = ImVec4{ button[0], button[1], button[2], button[3] };

		colors[ImGuiCol_ButtonHovered] = ImVec4{ buttonActive[0], buttonActive[1], buttonActive[2], buttonActive[3] };

		colors[ImGuiCol_WindowBg] = ImVec4{ background[0], background[1], background[2], background[3] };

		colors[ImGuiCol_TitleBg] = ImVec4{ titleBackground[0], titleBackground[1], titleBackground[2], titleBackground[3] };

		colors[ImGuiCol_TitleBgActive] = ImVec4{ titleBgActive[0], titleBgActive[1], titleBgActive[2], titleBgActive[3] };

		ImGui::End();
	}
};