#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletGraphics/mesh/meshManager.hpp"
#include "StarletGraphics/texture/textureManager.hpp"
#include "StarletGraphics/renderer/modelRenderer.hpp"
#include "StarletParser/utils/log.hpp"

#include "StarletScene/scene.hpp"

#include "StarletScene/components/model.hpp"
#include "StarletScene/components/transform.hpp"
#include "StarletScene/components/colour.hpp"
#include "StarletMath/mat4.hpp"

#include <glad/glad.h>

void ModelRenderer::bindSkyboxTexture(unsigned int textureID) const {
	glActiveTexture(GL_TEXTURE0 + SKYBOX_TU);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void ModelRenderer::updateModelUniforms(const Model& instance, const MeshCPU& data, const TransformComponent& transform, const ColourComponent& colour) const {
	const ModelUL& modelUL = uniforms.getModelCache().getModelUL();
	
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
void ModelRenderer::setModelIsSkybox(bool isSkybox) const {
	glUniform1i(uniforms.getModelCache().getModelUL().isSkybox, isSkybox ? 1 : 0);
}

bool ModelRenderer::drawModel(const Model& instance, const TransformComponent& transform, const ColourComponent& colour) const {
	if (!instance.isVisible) return true;

	const MeshCPU* cpuMesh{};
	if (!meshManager.getMeshCPU(instance.meshPath, cpuMesh))
		return error("Renderer", "drawModel", "Could not find mesh: " + instance.meshPath);

	updateModelUniforms(instance, *cpuMesh, transform, colour);
	const ModelUL& modelUL = uniforms.getModelCache().getModelUL();

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
bool ModelRenderer::drawOpaqueModels(const Scene& scene, const Vec3<float>& eye) const {
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

bool ModelRenderer::drawTransparentModels(const Scene& scene, const Vec3<float>& eye) const {
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
bool ModelRenderer::drawSkybox(const Model& skybox, const Vec3<float>& skyboxSize, const Vec3<float>& cameraPos) const {
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