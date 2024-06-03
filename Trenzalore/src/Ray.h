#pragma once

#include "glm\glm.hpp"

class Ray
{
public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	Ray() {};

	glm::vec3 GetOrigin() const{ return m_Origin; }
	glm::vec3 GetDirection() const{ return m_Direction; }

	glm::vec3 m_Origin{ 0.0f };
	glm::vec3 m_Direction{ 0.0f };
};

