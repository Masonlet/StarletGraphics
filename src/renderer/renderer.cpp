#include "StarletGraphics/manager/textureManager.hpp"
#include "StarletGraphics/manager/meshManager.hpp"
#include "StarletGraphics/manager/shaderManager.hpp"
#include "StarletGraphics/renderer/renderer.hpp"
#include "StarletParser/utils/log.hpp"

#include "StarletScene/scene.hpp"
#include "StarletScene/components/camera.hpp"
#include "StarletScene/components/model.hpp"
#include "StarletScene/components/transform.hpp"


#include "StarletGraphics/renderer/cameraView.hpp"
#include "StarletMath/mat4.hpp"

#include <glad/glad.h>

bool Renderer::init() {
	if (!glState.init()) return error("Renderer", "init", "Failed to initialize GL state");

	if (!uniforms.setProgram(glState.getProgram()))
		return error("Renderer", "init", "Failed to set UniformCache program");

	if (!uniforms.cacheAllLocations())
		return error("Renderer", "init", "Failed to cache uniform locations");

	return true;
}

void Renderer::renderFrame(const Scene& scene, const float aspect) const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const Camera* activeCam{};
	const TransformComponent* camTransform{};
	for (auto& pair : scene.getEntitiesOfType<Camera>()) {
		if (pair.second->enabled) {
			activeCam = pair.second;
			camTransform = &scene.getComponent<TransformComponent>(pair.first);
			break;
		}
	}

	if (!activeCam || !camTransform) return;

	const CameraView view = CameraView::fromTransform(camTransform->pos, camTransform->rot, WORLD_UP);
	cameraRenderer.updateCameraUniforms(view.eye, Mat4::lookAt(view.eye, view.front, view.up), Mat4::perspective(activeCam->fov, aspect, activeCam->nearPlane, activeCam->farPlane));
	lightRenderer.updateLightUniforms(glState.getProgram(), scene);

	modelRenderer.drawOpaqueModels(scene, view.eye);
	const Model* skyBoxModel = scene.getComponentByName<Model>(std::string("skybox"));
	const StarEntity skyboxEntity = scene.getEntityByName<Model>("skybox");
	if (skyBoxModel && skyboxEntity != -1) {
		const TransformComponent& skyBoxTransform = scene.getComponent<TransformComponent>(skyboxEntity);
		modelRenderer.drawSkybox(*skyBoxModel, skyBoxTransform.size, view.eye);
	}
	modelRenderer.drawTransparentModels(scene, view.eye);

	glBindVertexArray(0);
}