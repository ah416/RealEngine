#include "realpch.h"

#include "PerspectiveCamera.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(int windowWidth, int windowHeight, float left, float right, float top, float bottom)
	: m_CameraPos(1.0f, 0.0f, 1.0f), m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), m_WasPressed(false), m_LastX(0), m_LastY(0)
{
	RecalculateViewProjection();
}

// Called after every move or rotation of the camera
void PerspectiveCamera::RecalculateViewProjection()
{
	PROFILE_FUNCTION();

	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CameraFront = glm::normalize(front);

	// also re-calculate the Right and Up vector
	m_CameraRight = glm::normalize(glm::cross(m_CameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));

	// Calculate the view and projection matrices
	m_ViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	m_ProjectionMatrix = glm::perspectiveFov(glm::radians(m_CameraZoom), (float)m_WindowWidth, (float)m_WindowHeight, 0.1f, m_ViewDistance);

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

// Processes inputs from the keyboard
void PerspectiveCamera::ProcessKeyboardInput(Timestep timestep)
{
	PROFILE_FUNCTION();

	bool changed = false;

	if (Input::IsKeyPressed(REAL_KEY_W) && !Input::IsKeyPressed(REAL_KEY_LEFT_SHIFT))
	{
		m_CameraPos += m_CameraFront * m_CameraSpeed * timestep.GetSeconds();
		changed = true;
	}
	else if (Input::IsKeyPressed(REAL_KEY_S) && !Input::IsKeyPressed(REAL_KEY_LEFT_SHIFT))
	{
		m_CameraPos -= m_CameraFront * m_CameraSpeed * timestep.GetSeconds();
		changed = true;
	}
	if (Input::IsKeyPressed(REAL_KEY_A))
	{
		m_CameraPos -= m_CameraRight * m_CameraSpeed * timestep.GetSeconds();
		changed = true;
	}
	else if (Input::IsKeyPressed(REAL_KEY_D))
	{
		m_CameraPos += m_CameraRight * m_CameraSpeed * timestep.GetSeconds();
		changed = true;
	}

	if (Input::IsKeyPressed(REAL_KEY_W) && Input::IsKeyPressed(REAL_KEY_LEFT_SHIFT))
	{
		m_CameraPos += m_CameraFront * (m_CameraSpeed * 2) * timestep.GetSeconds();
		changed = true;
	}
	else if (Input::IsKeyPressed(REAL_KEY_S) && Input::IsKeyPressed(REAL_KEY_LEFT_SHIFT))
	{
		m_CameraPos -= m_CameraFront * (m_CameraSpeed * 2) * timestep.GetSeconds();
		changed = true;
	}

	if (changed)
		RecalculateViewProjection();
}

void PerspectiveCamera::ProcessMouseInput(float xPos, float yPos)
{
	PROFILE_FUNCTION();

	if (Input::IsMouseButtonPressed(REAL_MOUSE_BUTTON_2))
	{
		float xoffset;
		float yoffset;

		if (!m_WasPressed)
		{
			xoffset = 0;
			yoffset = 0;
			m_LastX = xPos;
			m_LastY = yPos;
			m_WasPressed = true;
		}
		else
		{
			xoffset = xPos - m_LastX;
			yoffset = m_LastY - yPos;
			m_LastX = xPos;
			m_LastY = yPos;
		}

		float sensitivity = 0.15f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_CameraFront = glm::normalize(direction);

		RecalculateViewProjection();
	}
	else
	{
		m_WasPressed = false;
	}
}
