#pragma once

#include "uniformCache.hpp"
#include "shader/shaderManager.hpp"
#include "mesh/meshManager.hpp"
#include "texture/textureManager.hpp"
#include <map>
#include <string>
#include <vector>

template <typename T> struct Vec3;
class Scene;
struct TransformComponent;
struct Mat4;
struct Mesh;
struct Model;
struct Camera;
struct Light;
struct Primitive;
struct Grid;
struct TextureData;

class Renderer {
public:
	Renderer(ShaderManager& sm, MeshManager& mm, TextureManager& tm) : shaderManager(sm), meshManager(mm), textureManager(tm) {}
	
	bool setProgram(const unsigned int program);
	unsigned int getProgram() const { return program; }

	bool initialize();

	void bindSkyboxTexture(const unsigned int texture) const;

	void updateModelUniforms(const TransformComponent& transform, const Model& instance, const MeshCPU& data) const;
	void setModelIsSkybox(const bool isSkybox) const;
	void updateCameraUniforms(const Vec3<float>& eye, const Mat4& view, const Mat4& projection) const;
	void updateLightUniforms(const Scene& scene) const;
	void updateLightCount(const int count) const;

	bool drawModel(const Model& instance, const TransformComponent& transform) const;
	bool drawOpaqueModels(const Scene& scene, const Vec3<float>& eye) const;
	bool drawTransparentModels(const Scene& scene, const Vec3<float>& eye) const;
	bool drawSkybox(const Model& skybox, const Vec3<float>& skyboxSize, const Vec3<float>& cameraPos) const;
	void renderFrame(const Scene& scene, const float aspect) const;

	void toggleWireframe();	

private:
	unsigned int program{ 0 };
	bool wireframe{ false };

	void setGLStateDefault();

	UniformCache uniforms;
	ShaderManager& shaderManager;
	MeshManager& meshManager;
	TextureManager& textureManager;
};