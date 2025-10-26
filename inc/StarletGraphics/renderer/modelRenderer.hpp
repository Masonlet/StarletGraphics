#pragma once

namespace Starlet {
	namespace Math {
		template <typename T> struct Vec3;
	}

	namespace Scene {
		class Scene;

		struct Model;

		struct TransformComponent;
		struct ColourComponent;
	}

	namespace Graphics {
		class UniformCache;
		class ResourceManager;

		struct MeshCPU;

		class ModelRenderer {
		public:
			ModelRenderer(const Graphics::UniformCache& uc, const Graphics::ResourceManager& rm) : uniforms(uc), resourceManager(rm) {}
			void updateModelUniforms(const Scene::Model& instance, const MeshCPU& data, const Scene::TransformComponent& transform, const Scene::ColourComponent& colour) const;

			void bindSkyboxTexture(const unsigned int texture) const;
			void setModelIsSkybox(const bool isSkybox) const;

			bool drawModel(const Scene::Model& instance, const Scene::TransformComponent& transform, const Scene::ColourComponent& colour) const;
			bool drawOpaqueModels(const Scene::Scene& scene, const Math::Vec3<float>& eye) const;
			bool drawTransparentModels(const Scene::Scene& scene, const Math::Vec3<float>& eye) const;
			bool drawSkybox(const Scene::Model& skybox, const Math::Vec3<float>& skyboxSize, const Math::Vec3<float>& cameraPos) const;

		private:
			const UniformCache& uniforms;
			const ResourceManager& resourceManager;
		};
	}
}