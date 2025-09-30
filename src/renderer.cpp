#include "StarletGraphics/shader/shaderManager.hpp"
#include "StarletGraphics/texture/textureManager.hpp"
#include "StarletGraphics/mesh/meshManager.hpp"

#include "StarletGraphics/renderer.hpp"
#include "StarletParser/utils/log.hpp"

#include "StarletScene/scene.hpp"

#include "StarletScene/components/textureData.hpp"
#include "StarletScene/components/model.hpp"
#include "StarletScene/components/light.hpp"
#include "StarletScene/components/camera.hpp"
#include "StarletScene/components/primitive.hpp"
#include "StarletScene/components/grid.hpp"

#include "StarletScene/components/transform.hpp"
#include "StarletScene/components/colour.hpp"

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

void Renderer::bindSkyboxTexture(unsigned int textureID) const {
	glActiveTexture(GL_TEXTURE0 + SKYBOX_TU);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void Renderer::updateModelUniforms(const Model& instance, const MeshCPU& data, const TransformComponent& transform, const ColourComponent& colour) const {
	const ModelUL& modelUL = uniforms.getModelUL();
	
	const Mat4 modelMat = Mat4::modelMatrix({ { transform.pos, 0.0f }, transform.rot, transform.size });
	glUniformMatrix4fv(modelUL.model, 1, GL_FALSE, modelMat.models);
	glUniformMatrix4fv(modelUL.modelInverseTranspose, 1, GL_FALSE, modelMat.inverse().transpose().models);

	glUniform4fv(modelUL.colourOverride, 1, &colour.colour.x);
	glUniform4fv(modelUL.specular, 1, &colour.specular.x);

	glUniform1i(modelUL.hasVertexColour, data.hasColours ? 1 : 0);
	glUniform2f(modelUL.yMinMax, data.minY, data.maxY);

	glUniform1i(modelUL.useTextures, instance.useTextures ? 1 : 0);
	glUniform1i(modelUL.colourMode, static_cast<int>(instance.mode));

	float r = 0.0f, g = 0.0f, b = 0.0f;
	int i = 0;
	for (unsigned char c : instance.name) {
		if (i % 3 == 0) r += static_cast<float>(c);
		else if (i % 3 == 1) g += static_cast<float>(c);
		else b += static_cast<float>(c);
		++i;
	}
	r = std::fmod(r / 255.0f, 1.0f);
	g = std::fmod(g / 255.0f, 1.0f);
	b = std::fmod(b / 255.0f, 1.0f);
	glUniform3f(modelUL.seed, r, g, b);
}
void Renderer::setModelIsSkybox(bool isSkybox) const {
	glUniform1i(uniforms.getModelUL().isSkybox, isSkybox ? 1 : 0);
}
void Renderer::updateCameraUniforms(const Vec3<float>& eye, const Mat4& view, const Mat4& projection) const {
	const ModelUL& modelUL = uniforms.getModelUL();
	glUniform3f(uniforms.getEyeLocation(), eye.x, eye.y, eye.z);
	glUniformMatrix4fv(modelUL.modelView, 1, GL_FALSE, view.ptr());
	glUniformMatrix4fv(modelUL.modelProj, 1, GL_FALSE, projection.ptr());
}
void Renderer::updateLightUniforms(const Scene& scene) const {
	auto lightEntities = scene.getEntitiesOfType<Light>();
	updateLightCount(static_cast<int>(lightEntities.size()));

	if (uniforms.getAmbientLightLocation() != -1) {
		const Vec4<float>& ambient = scene.getAmbientLight();
		glUniform4fv(uniforms.getAmbientLightLocation(), 1, &ambient.x);
	}

	int lightIndex = 0;
	for (const auto& lightPair : lightEntities) {
		const StarEntity entity = lightPair.first;
		const Light* light = lightPair.second;

		std::string prefix = "theLights[" + std::to_string(lightIndex) + "].";
		int pos_UL = glGetUniformLocation(program, (prefix + "position").c_str());
		int diff_UL = glGetUniformLocation(program, (prefix + "diffuse").c_str());
		int atten_UL = glGetUniformLocation(program, (prefix + "attenuation").c_str());
		int dir_UL = glGetUniformLocation(program, (prefix + "direction").c_str());
		int param1_UL = glGetUniformLocation(program, (prefix + "param1").c_str());
		int param2_UL = glGetUniformLocation(program, (prefix + "param2").c_str());

		if (!light->enabled || !scene.hasComponent<TransformComponent>(entity)) {
			if (param2_UL != -1) glUniform4f(param2_UL, 0.0f, 0.0f, 0.0f, 0.0f);
			++lightIndex;
			continue;
		}

		const TransformComponent& transform = scene.getComponent<TransformComponent>(entity);
		const ColourComponent& colour = scene.getComponent<ColourComponent>(entity);
		if (pos_UL != -1)    glUniform4f(pos_UL, transform.pos.x, transform.pos.y, transform.pos.z, 1.0f);
		if (diff_UL != -1)   glUniform4fv(diff_UL, 1, &colour.colour.r);
		if (atten_UL != -1)  glUniform4fv(atten_UL, 1, &light->attenuation.r);
		if (dir_UL != -1)    glUniform4f(dir_UL, transform.rot.r, transform.rot.g, transform.rot.b, 1.0f);
		if (param1_UL != -1) glUniform4f(param1_UL, static_cast<float>(light->type), light->param1.x, light->param1.y, 0.0f);
		if (param2_UL != -1) glUniform4f(param2_UL, light->enabled ? 1.0f : 0.0f, 0.0f, 0.0f, 0.0f);
		++lightIndex;
	}
}
void Renderer::updateLightCount(int count) const {
	if (uniforms.getLightCountLocation() != -1) glUniform1i(uniforms.getLightCountLocation(), count);
}

bool Renderer::drawModel(const Model& instance, const TransformComponent& transform, const ColourComponent& colour) const {
	if (!instance.isVisible) return true;

	const MeshCPU* cpuMesh{};
	if (!meshManager.getMeshCPU(instance.meshPath, cpuMesh))
		return error("Renderer", "drawModel", "Could not find mesh: " + instance.meshPath);

	updateModelUniforms(instance, *cpuMesh, transform, colour);
	const ModelUL& modelUL = uniforms.getModelUL();

	if (instance.useTextures) {
		glUniform4f(modelUL.texMixRatios, instance.textureMixRatio[0], instance.textureMixRatio[1], instance.textureMixRatio[2], instance.textureMixRatio[3]);

		for (size_t i = 0; i < instance.NUM_TEXTURES; ++i) {
			const std::string& name = instance.textureNames[i];
			if (name.empty()) continue;

			unsigned int textureID = textureManager.getTextureID(name);
			if (textureID == 0) return error("Renderer", "drawModel", "Could not find texture: " + name);

			glActiveTexture(GL_TEXTURE0 + static_cast<unsigned int>(i));
			glBindTexture(GL_TEXTURE_2D, textureID);
		}
	}

	glUniform1i(modelUL.isLit, instance.isLighted ? 1 : 0);

	const MeshGPU* gpuMesh{};
	if (!meshManager.getMeshGPU(instance.meshPath, gpuMesh))
		return error("Renderer", "drawModel", "Could not find mesh: " + instance.meshPath);

	if (colour.colour.w < 1.0f)	glDepthMask(GL_FALSE);
	glBindVertexArray(gpuMesh->VAOID);
	glDrawElements(GL_TRIANGLES, gpuMesh->numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	if (colour.colour.w < 1.0f) glDepthMask(GL_TRUE);

	return true;
}
bool Renderer::drawOpaqueModels(const Scene& scene, const Vec3<float>& eye) const {
	for (const auto& pair : scene.getEntitiesOfType<Model>()) {
		const StarEntity entity = pair.first;
		const Model* model = pair.second;

		if (model->name == "skybox") continue;

		if (!scene.hasComponent<TransformComponent>(entity)) continue;
		const TransformComponent& transform = scene.getComponent<TransformComponent>(entity);

		if(scene.hasComponent<ColourComponent>(entity)) {
			const ColourComponent& colour = scene.getComponent<ColourComponent>(entity);

			if (colour.colour.w < 1.0f) continue;
			if (!drawModel(*model, transform, colour))
				return error("Renderer", "drawModels", "Failed to draw opaque model");
		} else if (!drawModel(*model, transform, {}))
			return error("Renderer", "drawModels", "Failed to draw opaque model");
	}

	return true;
}

bool Renderer::drawTransparentModels(const Scene& scene, const Vec3<float>& eye) const {
	std::vector<std::tuple<const Model*, const TransformComponent*, const ColourComponent*>> transparentInstances;

	for (const auto& pair : scene.getEntitiesOfType<Model>()) {
		const StarEntity entity = pair.first;
		const Model* model = pair.second;
		
		if (model->name == "skybox") continue;

		if (!scene.hasComponent<TransformComponent>(entity)) continue;
		const TransformComponent& transform = scene.getComponent<TransformComponent>(entity);

		if (scene.hasComponent<ColourComponent>(entity)) {
			const ColourComponent& colour = scene.getComponent<ColourComponent>(entity);

			if (colour.colour.w < 1.0f)
				transparentInstances.push_back({ model, &transform, &colour });
		}
	}

	for (size_t i = 0; i < transparentInstances.size(); ++i) {
		for (size_t j = 0; j < transparentInstances.size() - i - 1; ++j) {
			const Vec3<float>& a = std::get<1>(transparentInstances[j])->pos;
			const Vec3<float>& b = std::get<1>(transparentInstances[j + 1])->pos;
			float distA = (a.x - eye.x) * (a.x - eye.x) + (a.y - eye.y) * (a.y - eye.y) + (a.z - eye.z) * (a.z - eye.z);
			float distB = (b.x - eye.x) * (b.x - eye.x) + (b.y - eye.y) * (b.y - eye.y) + (b.z - eye.z) * (b.z - eye.z);
			if (distA < distB) {
				std::tuple<const Model*, const TransformComponent*, const ColourComponent*> temp = transparentInstances[j];
				transparentInstances[j] = transparentInstances[j + 1];
				transparentInstances[j + 1] = temp;
			}
		}
	}

	for (const auto& pair : transparentInstances)
		if (!drawModel(*std::get<0>(pair), *std::get<1>(pair), *std::get<2>(pair)))
			return error("Renderer", "drawModels", "Failed to draw transparent model");

	return true;
}
bool Renderer::drawSkybox(const Model& skybox, const Vec3<float>& skyboxSize, const Vec3<float>& cameraPos) const {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDepthMask(GL_FALSE);
	Model tempSkybox = skybox;
	tempSkybox.isVisible = true;
	setModelIsSkybox(true);

	bindSkyboxTexture(textureManager.getTextureID(skybox.name));
	drawModel(tempSkybox, { cameraPos, { 0.0f, 0.0f, 0.0f }, skyboxSize }, {});

	setModelIsSkybox(false);
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
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

	updateCameraUniforms(eye, Mat4::lookAt(eye, front, up), Mat4::perspective(activeCam->fov, aspect, activeCam->nearPlane, activeCam->farPlane));

	updateLightUniforms(scene);

	drawOpaqueModels(scene, eye);

	const Model* skyBoxModel = scene.getComponentByName<Model>(std::string("skybox"));
	const StarEntity skyboxEntity = scene.getEntityByName<Model>("skybox");

	if (skyBoxModel && skyboxEntity != -1) {
		const TransformComponent& skyBoxTransform = scene.getComponent<TransformComponent>(skyboxEntity);
		drawSkybox(*skyBoxModel, skyBoxTransform.size, eye);
	}

	drawTransparentModels(scene, eye);

	glBindVertexArray(0);
}

void Renderer::toggleWireframe() {
	wireframe = !wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}