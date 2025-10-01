#pragma once

class UniformCache;
class MeshManager;
class TextureManager;

template <typename T> struct Vec3;
class Scene;

struct MeshCPU;
struct Model;

struct TransformComponent;
struct ColourComponent;

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