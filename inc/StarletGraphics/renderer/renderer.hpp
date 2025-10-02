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
	Renderer(MeshManager& mm, TextureManager& tm) : lightRenderer(uniforms), cameraRenderer(uniforms), modelRenderer(uniforms, mm, tm) {}

	bool init(const unsigned int program);
	void renderFrame(const unsigned int program, const Scene& scene, const float aspect) const;

private:
	UniformCache uniforms;
	LightRenderer lightRenderer;
	ModelRenderer modelRenderer;
	CameraRenderer cameraRenderer;
};