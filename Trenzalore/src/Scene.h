#pragma once

#include "Object.h"
#include"Material.h"

#include "glm\glm.hpp"

#include <vector>
#include <memory>

class Sphere : public Object
{
public:
	bool Intersect(const Ray& ray, float& t) const override
	{
		float a = glm::dot(ray.GetDirection(), ray.GetDirection());
		float b = 2.0f * glm::dot(ray.GetOrigin() - m_Position, ray.GetDirection());
		float c = glm::dot(ray.GetOrigin() - m_Position, ray.GetOrigin() - m_Position) - m_Radius * m_Radius;

		float discriminant = b * b - 4.0f * a * c;

		if (discriminant < 0.0f)
		{
			return false;
		}

		//float t1 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
		float t2 = (-b - glm::sqrt(discriminant)) / (2.0f * a);

		t = t2;

		return true;
	}

public:
	float m_Radius = 0.5f;
};

class Scene
{
public:

	Scene() = default;

	const std::vector<Object*>& Scene::GetObjects() const { return m_Objects; }
	const std::vector<Material>& Scene::GetMaterials() const { return m_Materials; }

	void Add(Object* sphere);
	void Clear();

public:
	std::vector<Object*> m_Objects;
	std::vector<Material> m_Materials;

};

