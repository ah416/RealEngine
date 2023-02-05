#pragma once

#include "Engine/Core/Event.h"
#include "Engine/Timestep.h"

class Layer
{
public:
	/*
	* A base class for "layers".
	* Pushed to the main application through the Application inherited class function AddLayer().
	* All pushed layers are updated every frame and event.
	* \param name: the name of the layer
	*/
	Layer(const std::string& name) : m_Name(name) {}

	// Called on attaching the layer
	virtual void OnAttach() {}

	virtual void OnDetach() {}

	// Called every frame
	virtual void OnUpdate(Timestep timestep) {}

	// Renders ImGui windows/viewports for each layer
	virtual void OnImGuiRender() {}

	// Called on event being passed to the layer
	virtual void OnEvent(Event& e) {}
private:
	const std::string m_Name;
};