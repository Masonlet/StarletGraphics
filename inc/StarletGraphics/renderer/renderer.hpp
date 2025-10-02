#pragma once

#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletGraphics/manager/glStateManager.hpp"

#include "StarletGraphics/renderer/lightRenderer.hpp"
#include "StarletGraphics/renderer/modelRenderer.hpp"
#include "StarletGraphics/renderer/cameraRenderer.hpp"

class ShaderManager;
class MeshManager;
class TextureManager;

class Scene;

class Renderer {
public:
	Renderer(ShaderManager& sm, MeshManager& mm, TextureManager& tm) : shaderManager(sm), lightRenderer(uniforms), cameraRenderer(uniforms), modelRenderer(uniforms, mm, tm), glState(sm) {}

	bool init();
	void renderFrame(const Scene& scene, const float aspect) const;
	void toggleWireframe() { glState.toggleWireframe(); }

private:
	GLStateManager glState;

	UniformCache uniforms;
	ShaderManager& shaderManager;

	LightRenderer lightRenderer;
	ModelRenderer modelRenderer;
	CameraRenderer cameraRenderer;
};