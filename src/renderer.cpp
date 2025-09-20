#include <glad/glad.h>
#include "StarletGraphics/shader/shaderManager.hpp"
#include "StarletGraphics/texture/textureManager.hpp"
#include "StarletGraphics/mesh/meshManager.hpp"
#include "StarletScene/objects/textureData.hpp"
#include "StarletScene/objects/textureConnection.hpp"
#include "StarletScene/objects/model.hpp"
#include "StarletScene/objects/primitive.hpp"
#include "StarletScene/objects/grid.hpp"
#include "StarletMath/mat4.hpp"
#include "StarletGraphics/renderer.hpp"
#include "StarletParsers/utils/log.hpp"
#include "StarletScene/objects/light.hpp"

void Renderer::setAssetPaths(const char* path) {
	shaderManager.setBasePath(path);
	meshManager.setBasePath(path);
	textureManager.setBasePath(path);
}

bool Renderer::createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath) {
	return shaderManager.createProgramFromPaths(name, vertPath, fragPath) ? true : error("Renderer", "createProgramFromPaths", "Failed to create program");
}
bool Renderer::setProgram(unsigned int program) {
	if (program == 0) return error("Renderer", "setProgram", "Program is 0");
	this->program = program;
	glUseProgram(program);

	return cacheUniformLocations() && debugLog("Renderer", "setProgram", "Successfully set shader program and located uniforms", true);
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

bool Renderer::cacheTextureUniforms() {
	glUniform1i(glGetUniformLocation(program, "textSampler2D_00"), 0);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_01"), 1);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_02"), 2);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_03"), 3);

	bool ok = true;
	ok &= getUniformLocation(modelUL.useTextures, "bUseTextures");
	ok &= getUniformLocation(modelUL.texMixRatios, "texMixRatios");
	ok &= getUniformLocation(skyboxTextureLocation, "skyboxCubeTexture");
	glUniform1i(skyboxTextureLocation, SKYBOX_TU);

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

bool Renderer::createTriangle(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour) {
	return meshManager.createTriangle(name, size, vertexColour)
		? debugLog("Renderer", "createTriangle", "Created triangle: " + name, true)
		: error("Renderer", "createTriangle", "Failed to create triangle: " + name);
}
bool Renderer::createSquare(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour) {
	return meshManager.createSquare(name, size, vertexColour)
		? debugLog("Renderer", "createSquare", "Created square: " + name, true)
		: error("Renderer", "createSquare", "Failed to create square: " + name);
}
bool Renderer::createCube(const std::string& name, const Vec3& size, const Vec4& vertexColour) {
	return meshManager.createCube(name, size, vertexColour)
		? debugLog("Renderer", "createCube", "Created cube: " + name, true)
		: error("Renderer", "createCube", "Failed to create cube: " + name);
}

bool Renderer::loadAndAddMesh(const std::string& path) {
	return meshManager.loadAndAddMesh(path) 
		? debugLog("Renderer", "loadAndAddMesh", "Added mesh: " + path, true)
		: error("Renderer", "loadAndAddMesh", "Failed to add mesh: " + path);
}
bool Renderer::addMesh(const std::string& path, MeshCPU& mesh) {
	return meshManager.addMesh(path, mesh) 
		? debugLog("Renderer", "addMesh", "Added mesh: " + path, true)
		: error("Renderer", "addMesh", "Failed to add mesh: " + path);
}
bool Renderer::addMeshes(const std::map<std::string, Model>& models) {
	for (const std::pair<const std::string, Model>& model : models)
		if (!loadAndAddMesh(model.second.meshPath))
			return error("Renderer", "addMeshes", "Failed to load/add mesh: " + model.second.meshPath);

	return true;
}

bool Renderer::getMesh(const std::string& path, MeshGPU*& dataOut) {
	return meshManager.getMeshGPU(path, dataOut);
}
bool Renderer::getMesh(const std::string& path, const MeshGPU*& dataOut) {
	return meshManager.getMeshGPU(path, dataOut);
}

void Renderer::updateModelUniforms(const Model& instance, const MeshCPU& data) const {
	const Mat4 modelMat = Mat4::modelMatrix(instance.transform);
	glUniformMatrix4fv(modelUL.model, 1, GL_FALSE, modelMat.models);
	glUniformMatrix4fv(modelUL.modelInverseTranspose, 1, GL_FALSE, modelMat.inverse().transpose().models);

	glUniform1i(modelUL.colourMode , static_cast<int>(instance.colourMode));
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
bool Renderer::drawModels(const std::map<std::string, Model>& instance, const Vec3& eye) const {
	std::vector<const Model*> transparentInstances;
	for (const std::pair<const std::string, Model>& model : instance) {
		const Model& instance = model.second;
		if (instance.name == "skybox") continue;

		if (instance.colour.w >= 1.0f) drawModel(instance);
		else                           transparentInstances.push_back(&instance);
	}

	for (size_t i = 0; i < transparentInstances.size(); ++i) {
		for (size_t j = 0; j < transparentInstances.size() - i - 1; ++j) {
			const Vec4& a = transparentInstances[j]->transform.pos;
			const Vec4& b = transparentInstances[j + 1]->transform.pos;
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

bool Renderer::drawSkybox(const Model& skybox, const Vec3& cameraPos) const {
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

void Renderer::setModelIsSkybox(bool isSkybox) const {
	glUniform1i(modelUL.isSkybox, isSkybox ? 1 : 0);
}

void Renderer::cacheLightUniformLocations(int maxLights) {
	if (maxLights <= 0) {
		lightUL.clear();
		return;
	}
	lightUL.resize(static_cast<size_t>(maxLights));

	for (int i = 0; i < maxLights; ++i) {
		std::string base = "theLights[" + std::to_string(i) + "].";
		lightUL[i].position = glGetUniformLocation(program, (base + "position").c_str());
		lightUL[i].diffuse = glGetUniformLocation(program, (base + "diffuse").c_str());
		lightUL[i].attenuation = glGetUniformLocation(program, (base + "attenuation").c_str());
		lightUL[i].direction = glGetUniformLocation(program, (base + "direction").c_str());
		lightUL[i].param1 = glGetUniformLocation(program, (base + "param1").c_str());
		lightUL[i].param2 = glGetUniformLocation(program, (base + "param2").c_str());
	}
}

void Renderer::updateLightCount(int count) const {
	if (lightCountLocation != -1) glUniform1i(lightCountLocation, count);
}

void Renderer::updateLightUniforms(const std::map<std::string, Light>& lights) const {
	const int n = static_cast<int>(lights.size());
	const int maxCached = static_cast<int>(lightUL.size());

	updateLightCount(n);

	std::map<std::string, Light>::const_iterator it = lights.begin();
	for (int i = 0; i < maxCached; ++i) {
		if (it != lights.end()) {
			const Light& L = it->second;
			if (lightUL[i].position != -1)    glUniform4f(lightUL[i].position, L.pos.x, L.pos.y, L.pos.z, 1.0f);
			if (lightUL[i].diffuse != -1)     glUniform4f(lightUL[i].diffuse, L.diffuse.r, L.diffuse.g, L.diffuse.b, L.diffuse.a);
			if (lightUL[i].attenuation != -1) glUniform4f(lightUL[i].attenuation, L.attenuation.r, L.attenuation.g, L.attenuation.b, L.attenuation.a);
			if (lightUL[i].direction != -1)   glUniform4f(lightUL[i].direction, L.direction.r, L.direction.g, L.direction.b, 1.0f);
			if (lightUL[i].param1 != -1)			glUniform4f(lightUL[i].param1, static_cast<float>(L.type), L.param1.x, L.param1.y, 0.0f);
			if (lightUL[i].param2 != -1)			glUniform4f(lightUL[i].param2, L.enabled ? 1.0f : 0.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (lightUL[i].param2 != -1) glUniform4f(lightUL[i].param2, 0.0f, 0.0f, 0.0f, 0.0f);
	}
}
void Renderer::updateCameraUniforms(const Vec3& eye, const Mat4& view, const Mat4& projection) const {
	glUniform3f(eyeLocation, eye.x, eye.y, eye.z);
	glUniformMatrix4fv(modelUL.modelView, 1, GL_FALSE, view.ptr());
	glUniformMatrix4fv(modelUL.modelProj, 1, GL_FALSE, projection.ptr());
}

void Renderer::bindSkyboxTexture(unsigned int textureID) const {
	glActiveTexture(GL_TEXTURE0 + SKYBOX_TU);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

bool Renderer::getUniformLocation(int& location, const char* name) const {
	location = glGetUniformLocation(program, name);
	if (location < 0) return error("Renderer", "getUniformLocation", std::string("Could not find uniform: ") + name);
	return true;
}

bool Renderer::addTexture(const std::string& name, const std::string& filePath) {
	return textureManager.addTexture(name, filePath)
		? debugLog("Renderer", "addTexture", "Added texture: " + filePath)
		: error("Renderer", "addTexture", "Failed to add texture: " + filePath);
}
bool Renderer::addTextureCube(const std::string& name, const std::string(&facePaths)[6]) {
	return textureManager.addTextureCube(name, facePaths) 
		? debugLog("Renderer", "addTextureCube", "Added texture cube: " + name)
		: error("Renderer", "addTexture", "Failed to add texture cube: " + name);
}
bool Renderer::addTextures(const std::map<std::string, TextureData>& textures) {
	for (const std::pair<const std::string, TextureData>& textureData : textures) {
		const TextureData& texture = textureData.second;

		if (!texture.isCube) {
			if(!addTexture(texture.name, texture.faces[0]))
				return error("Renderer", "loadSceneTextures", "Failed to load 2D texture: " + texture.name);
		} 
		else if (!addTextureCube(texture.name, texture.faces))
				return error("Renderer", "loadSceneTextures", "Failed to load cube map: " + texture.name);
	}

	return true;
}

void Renderer::toggleWireframe() {
	wireframe = !wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}