#pragma once

#include "Walnut/Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Scene.h"

#include "glm\glm.hpp"

#include <memory>

namespace Trenzalore {

	class Renderer
	{
	public:
		Renderer() = default;

		void Render(const Scene& scene, const Camera& camera);
		void OnCanvasResize(uint32_t width, uint32_t height);

		std::shared_ptr<Walnut::Image> GetDisplayImage() const { return m_DisplayImage; }
	private:
		glm::vec4 PerPixel(const Scene& scene, const Ray& ray);

	private:
		std::shared_ptr<Walnut::Image> m_DisplayImage;
		uint32_t* m_ImageBuffer = nullptr;
	};
}

