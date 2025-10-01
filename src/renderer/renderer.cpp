#include "StarletGraphics/texture/textureManager.hpp"
#include "StarletGraphics/mesh/meshManager.hpp"
#include "StarletGraphics/shader/shaderManager.hpp"
#include "StarletGraphics/renderer/renderer.hpp"
#include "StarletParser/utils/log.hpp"

#include "StarletScene/scene.hpp"

#include "StarletScene/components/camera.hpp"
#include "StarletScene/components/model.hpp"
#include "StarletScene/components/transform.hpp"
#include "StarletMath/mat4.hpp"

#include <glad/glad.h>

bool Renderer::setProgram(unsigned int program) {
	if (program == 0) return error("Renderer", "setProgram", "Program is 0");
	this->program = program;
	uniforms.setProgram(program);
	glUseProgram(program);
	return true;
}

bool Renderer::initialize() {
	if (!setProgram(shaderManager.getProgramID("shader1")))
		return error("Renderer", "setupShaders", "Failed to set program to shader1");

	if (!uniforms.cacheAllLocations())
		return error("Renderer", "setupShaders", "Failed to cache uniform locations");

	setGLStateDefault();
	return true;
}
void Renderer::setGLStateDefault() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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

	const Vec3<float> eye = camTransform->pos;
	const float yaw = camTransform->rot.y;
	const float pitch = camTransform->rot.x;

	Vec3<float> front;
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	front = front.normalized();

	Vec3<float> right = front.cross(WORLD_UP).normalized();
	Vec3<float> up = right.cross(front).normalized();

	cameraRenderer.updateCameraUniforms(eye, Mat4::lookAt(eye, front, up), Mat4::perspective(activeCam->fov, aspect, activeCam->nearPlane, activeCam->farPlane));
	lightRenderer.updateLightUniforms(program, scene);

	modelRenderer.drawOpaqueModels(scene, eye);
	const Model* skyBoxModel = scene.getComponentByName<Model>(std::string("skybox"));
	const StarEntity skyboxEntity = scene.getEntityByName<Model>("skybox");
	if (skyBoxModel && skyboxEntity != -1) {
		const TransformComponent& skyBoxTransform = scene.getComponent<TransformComponent>(skyboxEntity);
		modelRenderer.drawSkybox(*skyBoxModel, skyBoxTransform.size, eye);
	}
	modelRenderer.drawTransparentModels(scene, eye);

	glBindVertexArray(0);
}

void Renderer::toggleWireframe() {
	wireframe = !wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}