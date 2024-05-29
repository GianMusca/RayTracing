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
		m_ActiveScene = &scene;
		m_ActiveCamera = &camera;

		for (uint32_t y = 0; y < m_DisplayImage->GetHeight(); y++)
		{
			for (uint32_t x = 0; x < m_DisplayImage->GetWidth(); x++)
			{
				glm::vec4 color = CastRay(x, y);
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

	glm::vec4 Renderer::CastRay(uint32_t x, uint32_t y)
	{
		Ray ray;
		ray.m_Origin = m_ActiveCamera->GetPosition();
		ray.m_Direction = m_ActiveCamera->GetRayDirections()[x + y * m_DisplayImage->GetWidth()];

		HitRecord hitRecord = TraceRay(ray);

		if (hitRecord.HitScalar < 0.0f)
		{
			return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);
		glm::vec3 lightDirectionCond = glm::normalize(lightDirection);

		float d = glm::max(glm::dot(hitRecord.HitNormal, -lightDirectionCond), 0.0f);

		glm::vec3 color = hitRecord.Color * d;

		return glm::vec4(color, 1.0f);


	}
	HitRecord Renderer::TraceRay(const Ray& ray)
	{
		const Object* closestObject = nullptr;
		float closestT = FLT_MAX;

		for each (const auto & object in m_ActiveScene->GetObjects())
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
			return Miss();
		}

		return ClosestHit(ray, closestObject, closestT);

	}
	HitRecord Renderer::ClosestHit(const Ray& ray, const Object* object, float t)
	{
		HitRecord hitRecord;

		hitRecord.HitPoint = ray.GetOrigin() + t * ray.GetDirection();
		hitRecord.HitNormal = glm::normalize(hitRecord.HitPoint - object->m_Position);
		hitRecord.Color = object->m_Color;

		return hitRecord;
	}
	HitRecord Renderer::Miss()
	{
		HitRecord hitRecord;
		hitRecord.HitScalar = -1.0f;

		return hitRecord;
	}
}
