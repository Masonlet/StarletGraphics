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

struct Mat4;
struct MeshCPU;
struct Model;
struct Camera;
struct Light;
struct Primitive;
struct Grid;
struct TextureData;

struct TransformComponent;
struct ColourComponent;

class CameraRenderer {
public:
  CameraRenderer(const UniformCache& uc) : uniforms(uc) {}

	void updateCameraUniforms(const Vec3<float>& eye, const Mat4& view, const Mat4& projection) const;
private:
	const UniformCache& uniforms;
};

class ModelRenderer {
public:
	ModelRenderer(const UniformCache& uc, MeshManager& mm, TextureManager& tm) : uniforms(uc), meshManager(mm), textureManager(tm) {}
	void updateModelUniforms(const Model& instance, const MeshCPU& data, const TransformComponent& transform, const ColourComponent& colour) const;

	void bindSkyboxTexture(const unsigned int texture) const;
	void setModelIsSkybox(const bool isSkybox) const;

	bool drawModel(const Model& instance, const TransformComponent& transform, const ColourComponent& colour) const;
	bool drawOpaqueModels(const Scene& scene, const Vec3<float>& eye) const;
	bool drawTransparentModels(const Scene& scene, const Vec3<float>& eye) const;
	bool drawSkybox(const Model& skybox, const Vec3<float>& skyboxSize, const Vec3<float>& cameraPos) const;

private:
	const UniformCache& uniforms;
	MeshManager& meshManager;
	TextureManager& textureManager;
};

class LightRenderer {
public:
	LightRenderer(const UniformCache& uc) : uniforms(uc) {}

	void updateLightUniforms(const unsigned int program, const Scene& scene) const;
	void updateLightCount(const int count) const;
private:
	const UniformCache& uniforms;
};

class Renderer {
public:
	Renderer(ShaderManager& sm, MeshManager& mm, TextureManager& tm) : shaderManager(sm), uniforms(), lightRenderer(uniforms), cameraRenderer(uniforms), modelRenderer(uniforms, mm, tm) {}

	bool setProgram(const unsigned int program);
	unsigned int getProgram() const { return program; }

	bool initialize();
	void renderFrame(const Scene& scene, const float aspect) const;
	void toggleWireframe();

private:
	unsigned int program{ 0 };
	bool wireframe{ false };

	void setGLStateDefault();

	UniformCache uniforms;
	ShaderManager& shaderManager;

	LightRenderer lightRenderer;
	ModelRenderer modelRenderer;
	CameraRenderer cameraRenderer;
};