#pragma once

#include "StarletGraphics/shader/shaderManager.hpp"
#include "StarletGraphics/mesh/meshManager.hpp"
#include "StarletGraphics/texture/textureManager.hpp"
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

constexpr int SKYBOX_TU{ 20 };

struct ModelUL {
	int model{ -1 };
	int modelInverseTranspose{ -1 };
	int modelView{ -1 }, modelProj{ -1 };
	int isSkybox{ -1 };

	int colourMode{ -1 };
	int colourOverride{ -1 };
	int hasVertexColour{ -1 };
	int specular{ -1 };

	int yMinMax{ -1 };
	int seed{ -1 };

	int isLit{ -1 };

	int useTextures{ -1 };
	int texMixRatios{ -1 };
};

struct LightUL {
	int position_UL{ -1 };
	int diffuse_UL{ -1 };
	int attenuation_UL{ -1 };
	int direction_UL{ -1 };
	int param1_UL{ -1 };
	int param2_UL{ -1 };
};

class Renderer {
public:
	Renderer(ShaderManager& sm, MeshManager& mm, TextureManager& tm) : shaderManager(sm), meshManager(mm), textureManager(tm) {}

	void setAssetPaths(const char* path);

	bool setupShaders();
	void setGLStateDefault();

	bool setProgram(const unsigned int program);
	unsigned int getProgram() const { return program; }

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

	int eyeLocation{ -1 };
	int skyboxTextureLocation{ -1 };
	int lightCountLocation{ -1 }, ambientLightLocation{ -1 };

	bool cacheUniformLocations();
	bool cacheTextureUniforms();
	bool cacheCameraUniforms();
	bool cacheModelUniforms();
	bool cacheLightUniforms();

	bool getUniformLocation(int& location, const char* name) const;

	ModelUL modelUL;
	LightUL lightUL;

	ShaderManager& shaderManager;
	MeshManager& meshManager;
	TextureManager& textureManager;
};