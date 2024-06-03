#pragma once

#include <glm\glm.hpp>

class Material
{
public:
	float Roughness = 0.0f;
	float Metallic = 0.0f;
	glm::vec3 Albedo{ 0.0f };

};

