#include "Renderer.h"

#include <iostream>

namespace Utils {

	static uint32_t ConvertToRGBA(glm::vec4& color)
	{
		uint8_t red = (uint8_t)(color.r * 255.0f);
		uint8_t green = (uint8_t)(color.g * 255.0f);
		uint8_t blue = (uint8_t)(color.b * 255.0f);
		uint8_t alpha = (uint8_t)(color.a * 255.0f);

		uint32_t result = alpha << 24 | blue << 16 | green << 8 | red;
		return result;
	}
}

namespace Trenzalore {

	void Renderer::Render(const Scene& scene, const Camera& camera)
	{
		Ray ray;
		ray.m_Origin = camera.GetPosition();
		for (uint32_t y = 0; y < m_DisplayImage->GetHeight(); y++)
		{
			for (uint32_t x = 0; x < m_DisplayImage->GetWidth(); x++)
			{
				glm::vec2 coordNDC = { (float)x / (float)m_DisplayImage->GetWidth(), (float)y / (float)m_DisplayImage->GetHeight() };
				glm::vec2 coordScreen = coordNDC * 2.0f - 1.0f; // -1 -> 1

				ray.m_Direction = camera.GetRayDirections()[x + y * m_DisplayImage->GetWidth()];
				glm::vec4 color = PerPixel(scene, ray);
				color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
				m_ImageBuffer[x + y * m_DisplayImage->GetWidth()] = Utils::ConvertToRGBA(color);

			}

		}

		m_DisplayImage->SetData(m_ImageBuffer);
	}

	void Renderer::OnCanvasResize(uint32_t width, uint32_t height)
	{
		if (m_DisplayImage)
		{
			if (width == m_DisplayImage->GetWidth() && height == m_DisplayImage->GetHeight())
			{
				return;
			}
			m_DisplayImage->Resize(width, height);
		}
		else
		{
			m_DisplayImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
		}

		delete[] m_ImageBuffer;
		m_ImageBuffer = new uint32_t[width * height];

	}

	glm::vec4 Renderer::PerPixel(const Scene& scene, const Ray& ray)
	{
		glm::vec3 color(0, 0, 0);
		const Object * closestObject = nullptr;
		float closestT = FLT_MAX;

		for each (const auto& object in scene.GetObjects())
		{
			float t = 0.0f;
			bool hit = object->Intersect(ray, t);

			if (!hit)
			{
				continue;
			}

			else if (t < closestT)
			{
				closestObject = object;
				closestT = t;
			}
		}

		if (!closestObject)
		{
			return glm::vec4(0, 0, 0, 0);
		}

		glm::vec3 hitPoint = ray.GetOrigin() + closestT * ray.GetDirection();
		glm::vec3 normal = glm::normalize(hitPoint - closestObject->m_Position);

		glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);
		glm::vec3 lightDirectionCond = glm::normalize(lightDirection);

		float d = glm::max(glm::dot(normal, -lightDirectionCond),0.0f);

		color = closestObject->m_Color * d;

		return glm::vec4(color, 1.0f);

	}
}
