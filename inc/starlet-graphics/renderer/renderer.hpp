#pragma once

#include "starlet-graphics/uniform/uniform_cache.hpp"
#include "starlet-graphics/renderer/light_renderer.hpp"
#include "starlet-graphics/renderer/model_renderer.hpp"
#include "starlet-graphics/renderer/camera_renderer.hpp"

namespace Starlet {
	namespace Scene {
		class Scene;
	}

	namespace Graphics {
		class Renderer {
		public:
			Renderer(ResourceManager& rm) : lightRenderer(uniforms), cameraRenderer(uniforms), modelRenderer(uniforms, rm) {}

			bool init(const unsigned int program);
			void renderFrame(const unsigned int program, const Scene::Scene& scene, const float aspect) const;

		private:
			UniformCache uniforms;
			LightRenderer lightRenderer;
			ModelRenderer modelRenderer;
			CameraRenderer cameraRenderer;
		};
	}
}