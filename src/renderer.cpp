#include <glad/glad.h>
#include "StarletGraphics/shader/shaderManager.hpp"
#include "StarletGraphics/texture/textureManager.hpp"
#include "StarletGraphics/mesh/meshManager.hpp"
#include "StarletScene/components/textureData.hpp"
#include "StarletScene/components/textureConnection.hpp"
#include "StarletScene/components/model.hpp"
#include "StarletScene/components/camera.hpp"
#include "StarletScene/components/primitive.hpp"
#include "StarletScene/components/grid.hpp"
#include "StarletMath/mat4.hpp"
#include "StarletGraphics/renderer.hpp"
#include "StarletParsers/utils/log.hpp"
#include "StarletScene/components/light.hpp"

void Renderer::setAssetPaths(const char* path) {
	shaderManager.setBasePath(path);
	meshManager.setBasePath(path);
	textureManager.setBasePath(path);
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



bool Renderer::createPrimitiveMesh(const Primitive& primitive) {
	switch (primitive.type) {
	case PrimitiveType::Triangle:
		return createTriangle(primitive.name, { primitive.transform.size.x, primitive.transform.size.y }, primitive.colour);
	case PrimitiveType::Square:
		return createSquare(primitive.name, { primitive.transform.size.x, primitive.transform.size.y }, primitive.colour);
	case PrimitiveType::Cube:
		return createCube(primitive.name, primitive.transform.size, primitive.colour);
	default:
		return error("Renderer", "loadScenePrimitives", "Invalid primitive: " + primitive.name);
	}
}
bool Renderer::createGridMesh(const Grid& grid, const std::string& meshName) {
	switch (grid.type) {
	case GridType::Square:
		return createSquare(meshName, { grid.transform.size.x, grid.transform.size.y }, grid.colour);
	case GridType::Cube:
		return createCube(meshName, grid.transform.size, grid.colour);
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

	return debugLog("Renderer", "addMeshes", "Added " + std::to_string(models.size()) + " meshes", true);
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

	return debugLog("Renderer", "addTextures", "Added " + std::to_string(textures.size()) + " textures", true);
}
void Renderer::bindSkyboxTexture(unsigned int textureID) const {
	glActiveTexture(GL_TEXTURE0 + SKYBOX_TU);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}



void Renderer::updateModelUniforms(const Model& instance, const MeshCPU& data) const {
	const Mat4 modelMat = Mat4::modelMatrix(instance.transform);
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
void Renderer::updateLightUniforms(const std::vector<Light*>& lights) const {
	updateLightCount(static_cast<int>(lights.size()));

	for (const Light* light : lights) {
		if (light->enabled) {
			if (light->position_UL != -1)    glUniform4f(light->position_UL, light->pos.x, light->pos.y, light->pos.z, 1.0f);
			if (light->diffuse_UL != -1)     glUniform4f(light->diffuse_UL, light->diffuse.r, light->diffuse.g, light->diffuse.b, light->diffuse.a);
			if (light->attenuation_UL != -1) glUniform4f(light->attenuation_UL, light->attenuation.r, light->attenuation.g, light->attenuation.b, light->attenuation.a);
			if (light->direction_UL != -1)   glUniform4f(light->direction_UL, light->direction.r, light->direction.g, light->direction.b, 1.0f);
			if (light->param1_UL != -1)			glUniform4f(light->param1_UL, static_cast<float>(light->type), light->param1.x, light->param1.y, 0.0f);
			if (light->param2_UL != -1)			glUniform4f(light->param2_UL, light->enabled ? 1.0f : 0.0f, 0.0f, 0.0f, 0.0f);
			break;
		}
		else if (light->param2_UL != -1) glUniform4f(light->param2_UL, 0.0f, 0.0f, 0.0f, 0.0f);
	}
}
void Renderer::updateLightCount(int count) const {
	if (lightCountLocation != -1) glUniform1i(lightCountLocation, count);
}



bool Renderer::drawModel(const Model& instance) const {
	if (!instance.isVisible) return true;

	const MeshCPU* cpuMesh{};
	if (!meshManager.getMeshCPU(instance.meshPath, cpuMesh))
		return error("Renderer", "drawModel", "Could not find mesh: " + instance.meshPath);

	updateModelUniforms(instance, *cpuMesh);

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
bool Renderer::drawModels(const std::vector<Model*>& instance, const Vec3<float>& eye) const {
	std::vector<const Model*> transparentInstances;
	for (const Model* model : instance) {
		if (model->name == "skybox") continue;

		if (model->colour.w >= 1.0f) drawModel(*model);
		else                         transparentInstances.push_back(model);
	}

	for (size_t i = 0; i < transparentInstances.size(); ++i) {
		for (size_t j = 0; j < transparentInstances.size() - i - 1; ++j) {
			const Vec4<float>& a = transparentInstances[j]->transform.pos;
			const Vec4<float>& b = transparentInstances[j + 1]->transform.pos;
			float distA = (a.x - eye.x) * (a.x - eye.x) + (a.y - eye.y) * (a.y - eye.y) + (a.z - eye.z) * (a.z - eye.z);
			float distB = (b.x - eye.x) * (b.x - eye.x) + (b.y - eye.y) * (b.y - eye.y) + (b.z - eye.z) * (b.z - eye.z);
			if (distA < distB) {
				const Model* temp = transparentInstances[j];
				transparentInstances[j] = transparentInstances[j + 1];
				transparentInstances[j + 1] = temp;
			}
		}
	}

	for (const Model* instance : transparentInstances) 
		if(!drawModel(*instance))
			return error("Renderer", "drawModels", "Failed to draw transparent model: " + instance->name);

	return true;
}
bool Renderer::drawSkybox(const Model& skybox, const Vec3<float>& cameraPos) const {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDepthMask(GL_FALSE);

	Model tempSkybox = skybox;
	tempSkybox.transform.pos = { cameraPos, 0.0f };
	tempSkybox.isVisible = true;
	setModelIsSkybox(true);

	bindSkyboxTexture(textureManager.getTextureID(skybox.name));
	drawModel(tempSkybox);

	setModelIsSkybox(false);
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
	return true;
}

void Renderer::renderFrame(const Camera& cam, const float aspect, const std::vector<Light*>& lights, const std::vector<Model*>& models, const Model& skyBox) const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	updateCameraUniforms(cam.pos, Mat4::lookAt(cam.pos, cam.front), Mat4::perspective(cam.fov, aspect, cam.nearPlane, cam.farPlane));

	updateLightUniforms(lights);

	drawModels(models, cam.pos);
	drawSkybox(skyBox, cam.pos);

	glBindVertexArray(0);
}



void Renderer::toggleWireframe() {
	wireframe = !wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}