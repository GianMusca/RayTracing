#pragma once

#include "Ray.h"

class Object
{
public:
	virtual ~Object() = default;

	virtual bool Intersect(const Ray& ray, float& t) const = 0;
	glm::vec3 GetPosition() const { return m_Position; }
	int GetMaterial() const { return m_MaterialIndex; }

public:
	glm::vec3 m_Position{ 0.0f };
	int m_MaterialIndex = 0;
};

