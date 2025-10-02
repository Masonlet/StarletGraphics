#pragma once

#include "StarletGraphics/manager/meshManager.hpp"
#include "StarletGraphics/manager/textureManager.hpp"

struct Primitive;
struct Model;
struct TextureData;
struct Grid;

struct TransformComponent;
struct ColourComponent;

class Scene;
class SceneManager;
class ResourceManager;

class ResourceLoader {
public:
	ResourceLoader(ResourceManager& rm) : resourceManager(rm){};

	void setBasePath(const std::string& path);
	bool loadMeshes(const std::vector<Model*>& models);
	bool loadTextures(const std::vector<TextureData*>& textures);

	bool processTextureConnections(Scene& scene);
	bool processPrimitives(SceneManager& sm);
	bool processGrids(SceneManager& sm);

	bool createPrimitiveMesh(const Primitive& primitive, const TransformComponent& transform, const ColourComponent& colour);
	bool createGridMesh(const Grid& grid, const std::string& meshName, const TransformComponent& transform, const ColourComponent& colour);

private:
	std::string basePath;

	MeshManager meshManager;
	TextureManager textureManager;
	ResourceManager& resourceManager;
};