#pragma once

#include <map>
#include <string>
#include <vector>

class ShaderManager;
class MeshManager;
class TextureManager;
struct Vec3;
struct Mat4;
struct Mesh;
struct Model;
struct Light;

constexpr int SKYBOX_TU{ 20 };

class Renderer {
public:
	bool setProgram(unsigned int program);
	unsigned int getProgram() const { return program; }

	void updateModelUniforms(const Model& instance, const Mesh& data) const;

	void updateCameraUniforms(const Vec3& eye, const Mat4& view, const Mat4& projection) const;

	void cacheLightUniformLocations(int maxLights);
	void updateLightUniforms(const std::map<std::string, Light>& lights) const;
	void updateLightCount(int count) const;

	bool drawModel(MeshManager& meshManager, TextureManager& textureManager, const Model& instance) const;
	bool drawSkybox(MeshManager& meshManager, TextureManager& textureManager, const Model& skybox, const Vec3& cameraPos) const;

	void setModelIsSkybox(bool isSkybox) const;

	void bindSkyboxTexture(unsigned int texture) const;

private:
	unsigned int program{ 0 };

	int modelLocation{ -1 };
	int modelViewLocation{ -1 }, modelProjectionLocation{ -1 };
	int modelInverseTransposeLocation{ -1 };
	int eyeLocation{ -1 };
	int modelColourModeLocation{ -1 }, modelColourOverrideLocation{ -1 }, modelSpecularLocation{ -1 };
	int modelLightedLocation{ -1 }, modelHasVertColourLocation{ -1 };
	int modelMinYMaxYLocation{ -1 }, modelSeedLocation{ -1 };
	int modelUseTexturesLocation{ -1 }, modelTexMixRatiosLocation{ -1 };
	int modelIsSkyboxLocation{ -1 }, skyboxTextureLocation{ -1 };
	int lightCountLocation{ -1 }, ambientLightLocation{ -1 };

	bool cacheUniformLocations();
	bool cacheTextureUniforms();
	bool cacheCameraUniforms();
	bool cacheModelUniforms();
	bool cacheLightUniforms();

	bool getUniformLocation(int& location, const char* name) const;

	struct LightUL {
		int position{ -1 };
		int diffuse{ -1 };
		int attenuation{ -1 };
		int direction{ -1 };
		int param1{ -1 };
		int param2{ -1 };
	};
	std::vector<LightUL> lightUL;
};