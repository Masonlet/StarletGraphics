#pragma once

#include "StarletGraphics/shader/shaderManager.hpp"
#include "StarletGraphics/mesh/meshManager.hpp"
#include "StarletGraphics/texture/textureManager.hpp"
#include <map>
#include <string>
#include <vector>

struct Vec3;
struct Mat4;
struct Mesh;
struct Model;
struct Light;

constexpr int SKYBOX_TU{ 20 };

class Renderer {
public:
	void setAssetPaths(const char* path);

	bool createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath);
	bool setProgram(unsigned int program);
	unsigned int getProgram() const { return program; }
	unsigned int getProgramID(const std::string& name) const;

	bool createTriangle(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour);
	bool createSquare(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour);
	bool createCube(const std::string& name, const Vec3& size, const Vec4& vertexColour);

	bool addMesh(const std::string& path);
	bool addMesh(const std::string& path, Mesh& mesh);
	bool getMesh(const std::string& path, Mesh*& dataOut);
	bool getMesh(const std::string& path, const Mesh*& dataOut);

	void updateModelUniforms(const Model& instance, const Mesh& data) const;
	bool drawModel(const Model& instance) const;

	void bindSkyboxTexture(unsigned int texture) const;
	void setModelIsSkybox(bool isSkybox) const;
	bool drawSkybox(const Model& skybox, const Vec3& cameraPos) const;

	void updateCameraUniforms(const Vec3& eye, const Mat4& view, const Mat4& projection) const;

	void cacheLightUniformLocations(int maxLights);
	void updateLightUniforms(const std::map<std::string, Light>& lights) const;
	void updateLightCount(int count) const;

	bool addTexture(const std::string& name, const std::string& filePath);
	bool addTextureCube(const std::string& name, const std::string(&facePaths)[6]);

	void toggleWireframe();

private:
	ShaderManager shaderManager;
	MeshManager meshManager;
	TextureManager textureManager;

	unsigned int program{ 0 };
	bool wireframe{ false };

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