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
	int position{ -1 };
	int diffuse{ -1 };
	int attenuation{ -1 };
	int direction{ -1 };
	int param1{ -1 };
	int param2{ -1 };
};

class Renderer {
public:
	void setAssetPaths(const char* path);

	bool createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath);
	bool setProgram(const unsigned int program);
	unsigned int getProgram() const { return program; }
	unsigned int getProgramID(const std::string& name) const;

	bool createPrimitiveMesh(const Primitive& primitive);
	bool createGridMesh(const Grid& grid, const std::string& meshName);

	bool createTriangle(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour);
	bool createSquare(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour);
	bool createCube(const std::string& name, const Vec3& size, const Vec4& vertexColour);

	bool loadAndAddMesh(const std::string& path);
	bool addMesh(const std::string& path, MeshCPU& mesh);
	bool addMeshes(const std::map<std::string, Model>& models);

	bool getMesh(const std::string& path, MeshGPU*& dataOut);
	bool getMesh(const std::string& path, const MeshGPU*& dataOut);

	void updateModelUniforms(const Model& instance, const MeshCPU& data) const;
	bool drawModel(const Model& instance) const;
	bool drawModels(const std::map<std::string, Model>& instance, const Vec3& eye) const;

	void bindSkyboxTexture(const unsigned int texture) const;
	void setModelIsSkybox(const bool isSkybox) const;
	bool drawSkybox(const Model& skybox, const Vec3& cameraPos) const;

	void updateCameraUniforms(const Vec3& eye, const Mat4& view, const Mat4& projection) const;

	void cacheLightUniformLocations(const int maxLights);
	void updateLightUniforms(const std::map<std::string, Light>& lights) const;
	void updateLightCount(const int count) const;

	bool addTexture(const std::string& name, const std::string& filePath);
	bool addTextureCube(const std::string& name, const std::string(&facePaths)[6]);
	bool addTextures(const std::map<std::string, TextureData>& textures);

	void toggleWireframe();

	void renderFrame(const Camera& cam, const float aspect, const std::map<std::string, Light>& lights, const std::map<std::string, Model>& models, const Model& skyBox) const;

private:
	ShaderManager shaderManager;
	MeshManager meshManager;
	TextureManager textureManager;

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
	std::vector<LightUL> lightUL;
};