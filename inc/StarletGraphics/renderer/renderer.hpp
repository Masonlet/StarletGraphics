#pragma once

#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletGraphics/renderer/lightRenderer.hpp"
#include "StarletGraphics/renderer/modelRenderer.hpp"
#include "StarletGraphics/renderer/cameraRenderer.hpp"

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