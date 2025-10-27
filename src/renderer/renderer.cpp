#include "StarletGraphics/renderer/renderer.hpp"
#include "StarletGraphics/manager/textureManager.hpp"
#include "StarletGraphics/manager/meshManager.hpp"
#include "StarletGraphics/manager/shaderManager.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletScene/scene.hpp"
#include "StarletScene/component/camera.hpp"
#include "StarletScene/component/model.hpp"
#include "StarletScene/component/transform.hpp"
#include "StarletGraphics/renderer/cameraView.hpp"
#include "StarletMath/mat4.hpp"

#include <glad/glad.h>

namespace Starlet::Graphics {
	bool Renderer::init(const unsigned int program) {
		if (!uniforms.setProgram(program))
			return Serializer::error("Renderer", "init", "Failed to set UniformCache program");

		if (!uniforms.cacheAllLocations())
			return Serializer::error("Renderer", "init", "Failed to cache uniform locations");

		return true;
	}

	void Renderer::renderFrame(const unsigned int program, const Scene::Scene& scene, const float aspect) const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const Scene::Camera* activeCam{};
		const Scene::TransformComponent* camTransform{};
		for (auto& [entity, camera] : scene.getEntitiesOfType<Scene::Camera>()) {
			if (camera->enabled) {
				activeCam = camera;
				camTransform = &scene.getComponent<Scene::TransformComponent>(entity);
				break;
			}
		}

		if (!activeCam || !camTransform) return;

		const CameraView view = CameraView::fromTransform(camTransform->pos, camTransform->rot, WORLD_UP);
		cameraRenderer.updateCameraUniforms(view.eye, Math::Mat4::lookAt(view.eye, view.front, view.up), Math::Mat4::perspective(activeCam->fov, aspect, activeCam->nearPlane, activeCam->farPlane));
		lightRenderer.updateLightUniforms(program, scene);

		modelRenderer.drawOpaqueModels(scene, view.eye);
		const Scene::Model* skyBoxModel = scene.getComponentByName<Scene::Model>(std::string("skybox"));
		const Scene::Entity skyboxEntity = scene.getEntityByName<Scene::Model>("skybox");
		if (skyBoxModel && skyboxEntity != -1) {
			const Scene::TransformComponent& skyBoxTransform = scene.getComponent<Scene::TransformComponent>(skyboxEntity);
			modelRenderer.drawSkybox(*skyBoxModel, skyBoxTransform.size, view.eye);
		}
		modelRenderer.drawTransparentModels(scene, view.eye);

		glBindVertexArray(0);
	}
}