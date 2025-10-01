#pragma once

#include "StarletGraphics/uniform/uniformCache.hpp"

#include "StarletGraphics/renderer/lightRenderer.hpp"
#include "StarletGraphics/renderer/modelRenderer.hpp"
#include "StarletGraphics/renderer/cameraRenderer.hpp"

class ShaderManager;
class MeshManager;
class TextureManager;

class Scene;

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