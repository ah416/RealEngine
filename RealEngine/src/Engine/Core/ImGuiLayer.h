#pragma once

#include "Engine/Core/Event.h"
#include "Engine/Timestep.h"

#include <imgui.h>

namespace Colours
{
	namespace Theme
	{
		constexpr auto accent = IM_COL32(236, 158, 36, 255);
		constexpr auto highlight = IM_COL32(39, 185, 242, 255);
		constexpr auto niceBlue = IM_COL32(83, 232, 254, 255);
		constexpr auto compliment = IM_COL32(78, 151, 166, 255);
		constexpr auto background = IM_COL32(36, 36, 36, 255);
		constexpr auto backgroundDark = IM_COL32(26, 26, 26, 255);
		constexpr auto titlebar = IM_COL32(21, 21, 21, 255);
		constexpr auto propertyField = IM_COL32(15, 15, 15, 255);
		constexpr auto text = IM_COL32(192, 192, 192, 255);
		constexpr auto textBrighter = IM_COL32(210, 210, 210, 255);
		constexpr auto textDarker = IM_COL32(128, 128, 128, 255);
		constexpr auto muted = IM_COL32(77, 77, 77, 255);
		constexpr auto groupHeader = IM_COL32(47, 47, 47, 255);
		//constexpr auto selection				= IM_COL32(191, 177, 155, 255);
		//constexpr auto selectionMuted			= IM_COL32(59, 57, 45, 255);
		constexpr auto selection = IM_COL32(237, 192, 119, 255);
		constexpr auto selectionMuted = IM_COL32(237, 201, 142, 23);

		//constexpr auto backgroundPopup			= IM_COL32(63, 73, 77, 255); // in between
		//constexpr auto backgroundPopup			= IM_COL32(63, 77, 76, 255); // most green
		constexpr auto backgroundPopup = IM_COL32(63, 70, 77, 255); // most blue
	}
}

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	// Called on attach, initializes the layer
	virtual void OnAttach();

	// Called on detach, destroys the layer
	virtual void OnDetach();

	// Called before every calling any ImGui function
	virtual void Begin();

	// Called after completing ImGui functions
	virtual void End();

	// Handles events received to the layer
	virtual void OnEvent(Event& e);

private:
	// Whether or not to block events
	bool m_Blocking = true;
};