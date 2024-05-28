#pragma once

#include "glm\glm.hpp"

#include <vector>
#include <memory>

struct Sphere 
{
	glm::vec3 Position{ 0.0f };
	glm::vec3 Hit{ 0.0f };
	float Radius = 0.5f;

	glm::vec3 Color{1.0f, 0.0f, 0.0f};
};

class Scene
{
public:

	Scene() = default;

	std::vector<Sphere> Scene::GetObjects() const { return m_Objects; }
	void Add(Sphere sphere);
	void Clear();

	std::vector<Sphere> m_Objects;

private:

};

