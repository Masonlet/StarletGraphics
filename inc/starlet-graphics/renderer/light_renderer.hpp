#pragma once

namespace Starlet {
	namespace Scene {
		class Scene;
	}

	namespace Graphics {
		class UniformCache;

		class LightRenderer {
		public:
			LightRenderer(const UniformCache& uc) : uniforms(uc) {}

			void updateLightUniforms(const unsigned int program, const Scene::Scene& scene) const;
			void updateLightCount(const int count) const;
		private:
			const UniformCache& uniforms;
		};
	}
}
