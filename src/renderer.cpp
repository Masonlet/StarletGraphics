#include "StarletGraphics/shader/shaderManager.hpp"
#include "StarletGraphics/texture/textureManager.hpp"
#include "StarletGraphics/mesh/meshManager.hpp"
#include "StarletGraphics/renderer.hpp"
#include "StarletParsers/utils/log.hpp"

#include "StarletScene/scene.hpp"
#include "StarletScene/components/transform.hpp"
#include "StarletScene/components/textureData.hpp"
#include "StarletScene/components/textureConnection.hpp"
#include "StarletScene/components/model.hpp"
#include "StarletScene/components/light.hpp"
#include "StarletScene/components/camera.hpp"
#include "StarletScene/components/primitive.hpp"
#include "StarletScene/components/grid.hpp"

#include "StarletMath/mat4.hpp"

#include <glad/glad.h>


void Renderer::setAssetPaths(const char* path) {
	shaderManager.setBasePath(path);
	meshManager.setBasePath(path);
	textureManager.setBasePath(path);
}



bool Renderer::setupShaders() {
	if (!createProgramFromPaths("shader1", "vertex_shader.glsl", "fragment_shader.glsl"))
		return error("Renderer", "setupShaders", "Failed to create shader program from file");

	if (!setProgram(getProgramID("shader1")))
		return error("Renderer", "setupShaders", "Failed to set program to shader1");

	if (!cacheUniformLocations())
		return error("Renderer", "setupShaders", "Failed to cache uniform locations");

	setGLStateDefault();
	return debugLog("Renderer", "setupShaders", "Finished");
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



bool Renderer::createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath) {
	return shaderManager.createProgramFromPaths(name, vertPath, fragPath);
}
bool Renderer::setProgram(unsigned int program) {
	if (program == 0) return error("Renderer", "setProgram", "Program is 0");
	this->program = program;
	glUseProgram(program);
	return true;
}
unsigned int Renderer::getProgramID(const std::string& name) const {
	return shaderManager.getProgramID(name);
}



bool Renderer::cacheUniformLocations() {
	return cacheTextureUniforms()
		&& cacheCameraUniforms()
		&& cacheModelUniforms()
		&& cacheLightUniforms();
}

bool Renderer::getUniformLocation(int& location, const char* name) const {
	location = glGetUniformLocation(program, name);
	if (location < 0) return error("Renderer", "getUniformLocation", std::string("Could not find uniform: ") + name);
	return true;
}
bool Renderer::cacheTextureUniforms() {
	glUniform1i(glGetUniformLocation(program, "textSampler2D_00"), 0);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_01"), 1);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_02"), 2);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_03"), 3);

	bool ok = true;
	ok &= getUniformLocation(modelUL.useTextures, "bUseTextures");
	ok &= getUniformLocation(modelUL.texMixRatios, "texMixRatios");
	ok &= getUniformLocation(skyboxTextureLocation, "skyboxCubeTexture");
	if (skyboxTextureLocation != -1) glUniform1i(skyboxTextureLocation, SKYBOX_TU);

	return ok;
}
bool Renderer::cacheCameraUniforms() {
	return getUniformLocation(eyeLocation, "eyePos");
}
bool Renderer::cacheModelUniforms() {
	bool ok = true;
	ok &= getUniformLocation(modelUL.isSkybox, "bIsSkybox");
	ok &= getUniformLocation(modelUL.model, "mModel");
	ok &= getUniformLocation(modelUL.modelView, "mView");
	ok &= getUniformLocation(modelUL.modelProj, "mProj");
	ok &= getUniformLocation(modelUL.modelInverseTranspose, "mModel_InverseTranspose");
	ok &= getUniformLocation(modelUL.colourMode, "colourMode");
	ok &= getUniformLocation(modelUL.hasVertexColour, "hasVertexColour");
	ok &= getUniformLocation(modelUL.colourOverride, "colourOverride");
	ok &= getUniformLocation(modelUL.specular, "vertSpecular");
	ok &= getUniformLocation(modelUL.yMinMax, "yMin_yMax");
	ok &= getUniformLocation(modelUL.seed, "seed");
	ok &= getUniformLocation(modelUL.isLit, "bIsLit");
	return ok;
}
bool Renderer::cacheLightUniforms() {
	bool ok = true;
	ok &= getUniformLocation(lightCountLocation, "lightCount");
	ok &= getUniformLocation(ambientLightLocation, "ambientLight");
	glUniform4f(ambientLightLocation, 1.0, 1.0, 1.0, 1.0);
	return ok;
}



bool Renderer::createPrimitiveMesh(const Primitive& primitive, const TransformComponent& transform) {
	switch (primitive.type) {
	case PrimitiveType::Triangle:
		return createTriangle(primitive.name, { transform.size.x, transform.size.y }, primitive.colour);
	case PrimitiveType::Square:
		return createSquare(primitive.name, { transform.size.x, transform.size.y }, primitive.colour);
	case PrimitiveType::Cube:
		return createCube(primitive.name,transform.size, primitive.colour);
	default:
		return error("Renderer", "loadScenePrimitives", "Invalid primitive: " + primitive.name);
	}
}
bool Renderer::createGridMesh(const Grid& grid, const TransformComponent& transform, const std::string& meshName) {
	switch (grid.type) {
	case GridType::Square:
		return createSquare(meshName, { transform.size.x, transform.size.y }, grid.colour);
	case GridType::Cube:
		return createCube(meshName, transform.size, grid.colour);
	default:
		return error("Renderer", "createGridMesh", "Invalid grid: " + grid.name + ", mesh: " + meshName);
	}
}
bool Renderer::createTriangle(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour) {
	return meshManager.createTriangle(name, size, vertexColour);
}
bool Renderer::createSquare(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour) {
	return meshManager.createSquare(name, size, vertexColour);
}
bool Renderer::createCube(const std::string& name, const Vec3<float>& size, const Vec4<float>& vertexColour) {
	return meshManager.createCube(name, size, vertexColour);
}



bool Renderer::loadAndAddMesh(const std::string& path) {
	return meshManager.loadAndAddMesh(path);
}
bool Renderer::addMesh(const std::string& path, MeshCPU& mesh) {
	return meshManager.addMesh(path, mesh);
}
bool Renderer::addMeshes(const std::vector<Model*>& models) {
	for (const Model* model : models)
		if (!loadAndAddMesh(model->meshPath)) 
			return error("Renderer", "addMeshes", "Failed to load/add mesh: " + model->meshPath);

	return debugLog("Renderer", "addMeshes", "Added " + std::to_string(models.size()) + " meshes");
}
bool Renderer::getMesh(const std::string& path, MeshGPU*& dataOut) {
	return meshManager.getMeshGPU(path, dataOut);
}
bool Renderer::getMesh(const std::string& path, const MeshGPU*& dataOut) {
	return meshManager.getMeshGPU(path, dataOut);
}



bool Renderer::addTexture(const std::string& name, const std::string& filePath) {
	return textureManager.addTexture(name, filePath);
}
bool Renderer::addTextureCube(const std::string& name, const std::string(&facePaths)[6]) {
	return textureManager.addTextureCube(name, facePaths);
}
bool Renderer::addTextures(const std::vector<TextureData*>& textures) {
	for (const TextureData* texture : textures) {
		if (!texture->isCube) {
			if (!addTexture(texture->name, texture->faces[0]))
				return error("Renderer", "loadSceneTextures", "Failed to load 2D texture: " + texture->name);
		}
		else if (!addTextureCube(texture->name, texture->faces))
			return error("Renderer", "loadSceneTextures", "Failed to load cube map: " + texture->name);
	}

	return debugLog("Renderer", "addTextures", "Added " + std::to_string(textures.size()) + " textures");
}
void Renderer::bindSkyboxTexture(unsigned int textureID) const {
	glActiveTexture(GL_TEXTURE0 + SKYBOX_TU);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void Renderer::updateModelUniforms(const TransformComponent& transform, const Model& instance, const MeshCPU& data) const {
	const Mat4 modelMat = Mat4::modelMatrix({ { transform.pos, 0.0f }, transform.rot, transform.size });
	glUniformMatrix4fv(modelUL.model, 1, GL_FALSE, modelMat.models);
	glUniformMatrix4fv(modelUL.modelInverseTranspose, 1, GL_FALSE, modelMat.inverse().transpose().models);

	glUniform1i(modelUL.colourMode, static_cast<int>(instance.colourMode));
	glUniform4fv(modelUL.colourOverride, 1, &instance.colour.x);
	glUniform1i(modelUL.hasVertexColour, data.hasColours ? 1 : 0);
	glUniform4fv(modelUL.specular, 1, &instance.specular.x);

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
	glUniform2f(modelUL.yMinMax, data.minY, data.maxY);

	glUniform1i(modelUL.useTextures, instance.useTextures ? 1 : 0);
}
void Renderer::setModelIsSkybox(bool isSkybox) const {
	glUniform1i(modelUL.isSkybox, isSkybox ? 1 : 0);
}
void Renderer::updateCameraUniforms(const Vec3<float>& eye, const Mat4& view, const Mat4& projection) const {
	glUniform3f(eyeLocation, eye.x, eye.y, eye.z);
	glUniformMatrix4fv(modelUL.modelView, 1, GL_FALSE, view.ptr());
	glUniformMatrix4fv(modelUL.modelProj, 1, GL_FALSE, projection.ptr());
}
void Renderer::updateLightUniforms(const Scene& scene) const {
	auto lightEntities = scene.getEntitiesOfType<Light>();
	updateLightCount(static_cast<int>(lightEntities.size()));

	for (const auto& lightPair : lightEntities) {
		const StarEntity entity = lightPair.first;
		const Light* light = lightPair.second;

		if (!light->enabled) {
			if (light->param2_UL != -1) glUniform4f(light->param2_UL, 0.0f, 0.0f, 0.0f, 0.0f);
			continue;
		}

		if (!scene.hasComponent<TransformComponent>(entity)) continue;

		const TransformComponent& transform = scene.getComponent<TransformComponent>(entity);

		if (light->position_UL != -1)    glUniform4f(light->position_UL, transform.pos.x, transform.pos.y, transform.pos.z, 1.0f);
		if (light->diffuse_UL != -1)     glUniform4f(light->diffuse_UL, light->diffuse.r, light->diffuse.g, light->diffuse.b, light->diffuse.a);
		if (light->attenuation_UL != -1) glUniform4f(light->attenuation_UL, light->attenuation.r, light->attenuation.g, light->attenuation.b, light->attenuation.a);
		if (light->direction_UL != -1)   glUniform4f(light->direction_UL, transform.rot.r, transform.rot.g, transform.rot.b, 1.0f);
		if (light->param1_UL != -1)			glUniform4f(light->param1_UL, static_cast<float>(light->type), light->param1.x, light->param1.y, 0.0f);
		if (light->param2_UL != -1)			glUniform4f(light->param2_UL, light->enabled ? 1.0f : 0.0f, 0.0f, 0.0f, 0.0f);
		break;
	}
}
void Renderer::updateLightCount(int count) const {
	if (lightCountLocation != -1) glUniform1i(lightCountLocation, count);
}



bool Renderer::drawModel(const Model& instance, const TransformComponent& transform) const {
	if (!instance.isVisible) return true;

	const MeshCPU* cpuMesh{};
	if (!meshManager.getMeshCPU(instance.meshPath, cpuMesh))
		return error("Renderer", "drawModel", "Could not find mesh: " + instance.meshPath);

	updateModelUniforms(transform, instance, *cpuMesh);

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

	if (instance.colour.w < 1.0f)	glDepthMask(GL_FALSE);
	glBindVertexArray(gpuMesh->VAOID);
	glDrawElements(GL_TRIANGLES, gpuMesh->numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	if (instance.colour.w < 1.0f) glDepthMask(GL_TRUE);

	return true;
}
bool Renderer::drawOpaqueModels(const Scene& scene, const Vec3<float>& eye) const {
	for (const auto& pair : scene.getEntitiesOfType<Model>()) {
		const StarEntity entity = pair.first;
		const Model* model = pair.second;

		if (model->name == "skybox") continue;

		if (!scene.hasComponent<TransformComponent>(entity)) continue;
		const TransformComponent& transform = scene.getComponent<TransformComponent>(entity);

		if (model->colour.w >= 1.0f) drawModel(*model, transform);
	}

	return true;
}

bool Renderer::drawTransparentModels(const Scene& scene, const Vec3<float>& eye) const {
	std::vector<std::pair<const Model*, const TransformComponent*>> transparentInstances;

	for (const auto& pair : scene.getEntitiesOfType<Model>()) {
		const StarEntity entity = pair.first;
		const Model* model = pair.second;

		if (model->name == "skybox") continue;

		if (!scene.hasComponent<TransformComponent>(entity)) continue;
		const TransformComponent& transform = scene.getComponent<TransformComponent>(entity);

		if (model->colour.w < 1.0f) transparentInstances.push_back({model, &transform});
	}

	for (size_t i = 0; i < transparentInstances.size(); ++i) {
		for (size_t j = 0; j < transparentInstances.size() - i - 1; ++j) {
			const Vec3<float>& a = transparentInstances[j].second->pos;
			const Vec3<float>& b = transparentInstances[j + 1].second->pos;
			float distA = (a.x - eye.x) * (a.x - eye.x) + (a.y - eye.y) * (a.y - eye.y) + (a.z - eye.z) * (a.z - eye.z);
			float distB = (b.x - eye.x) * (b.x - eye.x) + (b.y - eye.y) * (b.y - eye.y) + (b.z - eye.z) * (b.z - eye.z);
			if (distA < distB) {
				std::pair<const Model*, const TransformComponent*> temp = transparentInstances[j];
				transparentInstances[j] = transparentInstances[j + 1];
				transparentInstances[j + 1] = temp;
			}
		}
	}

	for (const auto& pair : transparentInstances)
		if (!drawModel(*pair.first, *pair.second))
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

	TransformComponent tempTransform;
	tempTransform.pos = cameraPos;
	tempTransform.size = skyboxSize;
	drawModel(tempSkybox, tempTransform);

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

	const float yaw = camTransform->rot.y;
	const float pitch = camTransform->rot.x;

	Vec3<float> front;
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	front = front.normalized();

	Vec3<float> right = front.cross(WORLD_UP).normalized();
	Vec3<float> up = right.cross(front).normalized();

	updateCameraUniforms(camTransform->pos, Mat4::lookAt(camTransform->pos, front, up), Mat4::perspective(activeCam->fov, aspect, activeCam->nearPlane, activeCam->farPlane));

	updateLightUniforms(scene);

	drawOpaqueModels(scene, camTransform->pos);

	const Model* skyBoxModel = scene.getComponentByName<Model>(std::string("skybox"));
	const StarEntity skyboxEntity = scene.getEntityByName<Model>("skybox");

	if (skyBoxModel && skyboxEntity != -1) {
		const TransformComponent& skyBoxTransform = scene.getComponent<TransformComponent>(skyboxEntity);
		drawSkybox(*skyBoxModel, skyBoxTransform.size, camTransform->pos);
	}

	drawTransparentModels(scene, camTransform->pos);

	glBindVertexArray(0);
}



void Renderer::toggleWireframe() {
	wireframe = !wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}