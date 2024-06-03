#include "Renderer.h"

#include "Walnut\Random.h"

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

		delete[] m_AccumulationBuffer;
		m_AccumulationBuffer = new glm::vec4[width * height];

	}

	glm::vec4 Renderer::CastRay(uint32_t x, uint32_t y)
	{
		Ray ray;
		ray.m_Origin = m_ActiveCamera->GetPosition();
		ray.m_Direction = m_ActiveCamera->GetRayDirections()[x + y * m_DisplayImage->GetWidth()];

		int depth = 6;
		glm::vec3 color(0.0f, 0.0f, 0.0f);
		float multiplier = 1.0f;

		for (int i = 0; i < depth; i++) 
		{
			HitRecord hitRecord = TraceRay(ray);

			if (hitRecord.HitScalar < 0.0f)
			{
				color += glm::vec3(0.6f, 0.7f, 0.9f) * multiplier;
				break;
			}

			const Object* object = m_ActiveScene->GetObjects()[hitRecord.ObjectIndex];
			const Material& material = m_ActiveScene->GetMaterials()[object->m_MaterialIndex];

			glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);
			glm::vec3 lightDirectionCond = glm::normalize(lightDirection);
			float intensity = glm::max(glm::dot(hitRecord.HitNormal, -lightDirectionCond), 0.0f);


			glm::vec3 objectColor = material.Albedo;

			objectColor *= intensity;
			color += objectColor * multiplier;

			multiplier *= 0.5f;

			ray.m_Origin = hitRecord.HitPoint + hitRecord.HitNormal * 0.0001f;
			ray.m_Direction = glm::reflect(ray.m_Direction, hitRecord.HitNormal + material.Roughness * Walnut::Random::Vec3(-0.5f, 0.5f));
		}

		return glm::vec4(color, 1.0f);

	}

	HitRecord Renderer::TraceRay(const Ray& ray)
	{
		int closestObjectIndex = -1;
		float closestT = FLT_MAX;
		const std::vector<Object*>& objects = m_ActiveScene->GetObjects();

		for(int i = 0; i < objects.size(); i++)
		{
			float t = 0.0f;
			bool hit = objects[i]->Intersect(ray, t);

			if (!hit)
			{
				continue;
			}

			else if (t > 0.0f &&  t < closestT)
			{
				closestObjectIndex = i;
				closestT = t;
			}
		}

		if (closestObjectIndex < 0.0f)
		{
			return Miss();
		}

		return ClosestHit(ray, closestT, closestObjectIndex);

	}

	HitRecord Renderer::ClosestHit(const Ray& ray, float t, int objectIndex)
	{
		const Object* closestObject = m_ActiveScene->GetObjects()[objectIndex];

		HitRecord hitRecord;
		hitRecord.HitPoint = ray.GetOrigin() + t * ray.GetDirection();
		hitRecord.HitNormal = glm::normalize(hitRecord.HitPoint - closestObject->m_Position);
		hitRecord.ObjectIndex = objectIndex;

		return hitRecord;
	}
	HitRecord Renderer::Miss()
	{
		HitRecord hitRecord;
		hitRecord.HitScalar = -1.0f;

		return hitRecord;
	}
}
