#pragma once

#include "Engine/Core/Event.h"
#include "Engine/Timestep.h"

#include <glm/glm.hpp>

class PerspectiveCamera
{
public:
	// Creates a perspective camera, takes the left, right, top and bottom size of the camera, as well as the size of the rendered window
	PerspectiveCamera(int windowWidth, int windowHeight, float left = -2.0f, float right = 2.0f, float top = -2.0f, float bottom = 2.0f);
	
	// Recalculates the view, projection and view projection matrices
	void RecalculateViewProjection();

	// Processes inputs from the keyboard that may affect the camera
	void ProcessKeyboardInput(Timestep timestep);

	// Processes inputs from the mouse that may affect the camera
	void ProcessMouseInput(float xPos, float yPos);

	void SetPosition(const glm::vec3& pos) { m_CameraPos = pos; RecalculateViewProjection(); }

	const glm::vec3 GetCameraPos() const { return m_CameraPos; }

	void SetMovementSpeed(const float speed) { m_CameraSpeed = speed; }
	const float GetMovementSpeed() const { return m_CameraSpeed; }

	void SetFront(const glm::vec3& front) { m_CameraFront = front; }
	const glm::vec3 GetFront() const { return m_CameraFront; }

	void SetRight(const glm::vec3& right) { m_CameraRight = right; }
	const glm::vec3 GetRight() const { return m_CameraRight; }

	void SetWindowWidth(const int width) { m_WindowWidth = width; }
	void SetWindowHeight(const int height) { m_WindowHeight = height; }

	int GetWindowWidth() const { return m_WindowWidth; }
	int GetWindowHeight() const { return m_WindowHeight; }

	inline glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
	inline glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
	inline glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

private:
	// The actual camera position
	glm::vec3 m_CameraPos;

	// Variables that control the speed of camera movement as well as the "zoom" level (fov?) of the camera
	float m_CameraSpeed = 1.0f;
	float m_CameraZoom = 75.0f;

	// Unit vector of the camera's direction
	glm::vec3 m_CameraFront = { 0.0f, 0.0f, -1.0f };

	glm::vec3 m_CameraRight = glm::vec3(1.0f);
	glm::vec3 m_CameraUp = { 0.0f, 1.0f, 0.0f };

	float m_ViewDistance = 1000.0f;

	// The view, projection and view projection matrices
	glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
	
	// Stores the window width and height for matrix calculations // TODO: CHANGE TO BE CAMERA RATHER THAN WINDOW
	int m_WindowWidth, m_WindowHeight;

	// Was mouse button 1 pressed last frame?
	bool m_WasPressed;

	// Mouse related variables, holds pitch and yaw as well as the mouse's last coordinates
	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;
	float m_LastX, m_LastY;
};