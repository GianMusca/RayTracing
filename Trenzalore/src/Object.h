#pragma once

#include "Ray.h"

class Object
{
public:
	virtual ~Object() = default;

	virtual bool Intersect(const Ray& ray, float& t) const = 0;

public:
	glm::vec3 m_Position{ 0.0f };
	glm::vec3 m_Color{ 1.0f, 0.0f, 0.0f };
};

