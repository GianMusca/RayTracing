#pragma once

#include <glm\glm.hpp>

#include<vector>

class Camera
{
public:
	Camera(float verticalFOV, float nearPlane, float farPlane);

	bool Update(float ts);
	void OnUpdate(float ts);
	void OnCanvasResize(uint32_t width, uint32_t height);

	const glm::vec3 GetPosition() const { return m_Position; }
	const std::vector<glm::vec3>& GetRayDirections() const { return m_RayDirections; }

private:
	void CalculateViewMatrix();
	void CalculatePerspectiveMatrix();
	void CalculateRayDirections();

	bool ProcessKeyboardInput(float deltaTime);
	bool ProcessMouseMovement(glm::vec2 mouseDelta);


private:
	glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Direction{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Up{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Right{ 0.0f, 0.0f, 0.0f };
	const glm::vec3 m_WorldUp{ 0.0f, 1.0f, 0.0f };

	glm::mat4 m_ViewMatrix{ 1.0f };
	glm::mat4 m_InverseViewMatrix{ 1.0f };
	glm::mat4 m_PerspectiveMatrix{ 1.0f };
	glm::mat4 m_InversePerspectiveMatrix{ 1.0f };

	float m_VerticalFOV = 45.0f;
	float m_NearClipPlane = 0.1f;
	float m_FarClipPlane = 100.0f;

	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;

	float m_MovementSpeed = 5.0f;
	float m_RotationSpeed = 0.3f;

	glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };
	float m_MouseSensitivity = 0.002f;

	std::vector<glm::vec3> m_RayDirections;

};

