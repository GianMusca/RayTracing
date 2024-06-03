#pragma once

#include "Walnut/Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Scene.h"

#include "glm\glm.hpp"

#include <memory>

struct HitRecord
{
	glm::vec3 HitNormal{ 0.0f };
	glm::vec3 HitPoint{ 0.0f };
	float HitScalar = 0.0f;
	
	int ObjectIndex = 0;
};


namespace Trenzalore {
	struct Settings 
	{
		bool Accumulate = true;
	};

	class Renderer
	{
	public:
		Renderer() = default;

		void Render(const Scene& scene, const Camera& camera);
		void OnCanvasResize(uint32_t width, uint32_t height);

		void ResetFrameIndex() { m_FrameIndex = 1; }

		std::shared_ptr<Walnut::Image> GetDisplayImage() const { return m_DisplayImage; }
		Settings& GetSettings() { return m_Settings; }

	private:
		glm::vec4 CastRay(uint32_t x, uint32_t y);
		HitRecord TraceRay(const Ray& ray);
		HitRecord ClosestHit(const Ray& ray, float t, int objectIndex);
		HitRecord Miss();
		

	private:
		std::shared_ptr<Walnut::Image> m_DisplayImage;
		uint32_t* m_ImageBuffer = nullptr;
		glm::vec4* m_AccumulationBuffer = nullptr;

		const Scene* m_ActiveScene = nullptr;
		const Camera* m_ActiveCamera = nullptr;

		Settings m_Settings;
		int m_FrameIndex = 1;
	};
}

