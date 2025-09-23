#pragma once

#include "StarletGraphics/shader/shaderManager.hpp"
#include "StarletGraphics/mesh/meshManager.hpp"
#include "StarletGraphics/texture/textureManager.hpp"
#include <map>
#include <string>
#include <vector>

template <typename T> struct Vec3;
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

class Renderer {
public:
	void setAssetPaths(const char* path);

	bool createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath);
	bool setProgram(const unsigned int program);
	unsigned int getProgram() const { return program; }
	unsigned int getProgramID(const std::string& name) const;

	bool cacheUniformLocations();

	bool createPrimitiveMesh(const Primitive& primitive);
	bool createGridMesh(const Grid& grid, const std::string& meshName);
	bool createTriangle(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour);
	bool createSquare(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour);
	bool createCube(const std::string& name, const Vec3<float>& size, const Vec4<float>& vertexColour);

	bool loadAndAddMesh(const std::string& path);
	bool addMesh(const std::string& path, MeshCPU& mesh);
	bool addMeshes(const std::vector<Model*>& models);
	bool getMesh(const std::string& path, MeshGPU*& dataOut);
	bool getMesh(const std::string& path, const MeshGPU*& dataOut);

	bool addTexture(const std::string& name, const std::string& filePath);
	bool addTextureCube(const std::string& name, const std::string(&facePaths)[6]);
	bool addTextures(const std::vector<TextureData*>& textures);
	void bindSkyboxTexture(const unsigned int texture) const;

	void updateModelUniforms(const Model& instance, const MeshCPU& data) const;
	void setModelIsSkybox(const bool isSkybox) const;
	void updateCameraUniforms(const Vec3<float>& eye, const Mat4& view, const Mat4& projection) const;
	void updateLightUniforms(const std::vector<Light*>& lights) const;
	void updateLightCount(const int count) const;

	bool drawModel(const Model& instance) const;
	bool drawModels(const std::vector<Model*>& instance, const Vec3<float>& eye) const;
	bool drawSkybox(const Model& skybox, const Vec3<float>& cameraPos) const;
	void renderFrame(const Camera& cam, const float aspect, const std::vector<Light*>& lights, const std::vector<Model*>& models, const Model& skyBox) const;

	void toggleWireframe();

private:
	unsigned int program{ 0 };
	bool wireframe{ false };

	int eyeLocation{ -1 };
	int skyboxTextureLocation{ -1 };
	int lightCountLocation{ -1 }, ambientLightLocation{ -1 };

	bool cacheTextureUniforms();
	bool cacheCameraUniforms();
	bool cacheModelUniforms();
	bool cacheLightUniforms();

	bool getUniformLocation(int& location, const char* name) const;

	ModelUL modelUL;

	ShaderManager shaderManager;
	MeshManager meshManager;
	TextureManager textureManager;
};