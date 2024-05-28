#include "Camera.h"
#include "Walnut\Input\Input.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(float verticalFOV, float nearPlane, float farPlane)
	: m_VerticalFOV(verticalFOV), m_NearClipPlane(nearPlane), m_FarClipPlane(farPlane)
{
	m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
	m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	m_Up = m_WorldUp;
	m_Right = glm::cross(m_Direction, m_Up);

}

bool Camera::Update(float ts)
{
	glm::vec2 mousePos = Walnut::Input::GetMousePosition();
	glm::vec2 mouseDelta = (mousePos - m_LastMousePosition) * m_MouseSensitivity;
	m_LastMousePosition = mousePos;

	if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Right))
	{
		Walnut::Input::SetCursorMode(Walnut::CursorMode::Normal);
		return false;
	}

	Walnut::Input::SetCursorMode(Walnut::CursorMode::Locked);

	bool moved = false;

	m_Right = glm::cross(m_Direction, m_WorldUp);

	moved = ProcessKeyboardInput(ts);
	moved = ProcessMouseMovement(mouseDelta);

	if (moved)
	{
		CalculateViewMatrix();
		CalculateRayDirections();
	}

	return moved;
}

void Camera::OnCanvasResize(uint32_t width, uint32_t height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight)
	{
		return;
	}

	m_ViewportWidth = width;
	m_ViewportHeight = height;

	CalculatePerspectiveMatrix();
	CalculateRayDirections();
	
}

void Camera::CalculateViewMatrix()
{
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, m_WorldUp);
	m_InverseViewMatrix = glm::inverse(m_ViewMatrix);

}

void Camera::CalculatePerspectiveMatrix()
{
	m_PerspectiveMatrix = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClipPlane, m_FarClipPlane);
	m_InversePerspectiveMatrix = glm::inverse(m_PerspectiveMatrix);
}

void Camera::CalculateRayDirections()
{
	m_RayDirections.resize(m_ViewportWidth * m_ViewportHeight);

	for (uint32_t y = 0; y < m_ViewportHeight; y++)
	{
		for (uint32_t x = 0; x < m_ViewportWidth; x++)
		{
			glm::vec2 coordNDC = { (float)x / (float)m_ViewportWidth, (float)y / (float)m_ViewportHeight };
			glm::vec2 coordScreen = coordNDC * 2.0f - 1.0f; // -1 -> 1

			glm::vec4 cameraSpace = m_InversePerspectiveMatrix * glm::vec4(coordScreen.x, coordScreen.y, 1, 1);
			glm::vec3 rayDirection = glm::vec3(m_InverseViewMatrix * glm::vec4(glm::normalize(glm::vec3(cameraSpace) / cameraSpace.w), 0)); // World space

			m_RayDirections[x + y * m_ViewportWidth] = rayDirection;

		}
	}
}

bool Camera::ProcessKeyboardInput(float deltaTime)
{
	bool moved = false;

	if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W))
	{
		m_Position += m_Direction * m_MovementSpeed * deltaTime;
		moved = true;
	}
	else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S))
	{
		m_Position -= m_Direction * m_MovementSpeed * deltaTime;
		moved = true;
	}
	if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A))
	{
		m_Position -= m_Right * m_MovementSpeed * deltaTime;
		moved = true;
	}
	else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D))
	{
		m_Position += m_Right * m_MovementSpeed * deltaTime;
		moved = true;
	}
	if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Q))
	{
		m_Position -= m_WorldUp * m_MovementSpeed * deltaTime;
		moved = true;
	}
	else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::E))
	{
		m_Position += m_WorldUp * m_MovementSpeed * deltaTime;
		moved = true;
	}
	if (Walnut::Input::IsKeyDown(Walnut::KeyCode::B))
	{
		m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
		m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		moved = true;
	}

	return moved;
}

bool Camera::ProcessMouseMovement(glm::vec2 mouseDelta)
{
	bool moved = false;

	if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
	{
		float pitchDelta = mouseDelta.y * m_RotationSpeed;
		float yawDelta = mouseDelta.x * m_RotationSpeed;

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, m_Right),
			glm::angleAxis(-yawDelta,m_WorldUp)));
		m_Direction = glm::rotate(q, m_Direction);

		moved = true;
	}

	return moved;
}