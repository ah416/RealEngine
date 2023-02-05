#pragma once

#include "Engine/Core/ImGuiLayer.h"
#include "Engine/Core/Event.h"
#include "Engine/Timestep.h"

#include <imgui.h>

class ImGuiLayerVulkan : public ImGuiLayer
{
public:
	ImGuiLayerVulkan();
	~ImGuiLayerVulkan();

	// Called on attach, initializes the layer
	virtual void OnAttach() override;

	// Called on detach, destroys the layer
	virtual void OnDetach() override;

	// Called before every calling any ImGui function
	virtual void Begin() override;

	// Called after completing ImGui functions
	virtual void End() override;

	// Handles events received to the layer
	virtual void OnEvent(Event& e) override;

private:
	// Whether or not to block events
	bool m_Blocking = true;
};