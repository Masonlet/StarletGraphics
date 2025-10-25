#pragma once

#include "StarletGraphics/manager/meshManager.hpp"
#include "StarletGraphics/factory/meshFactory.hpp"
#include "StarletGraphics/manager/textureManager.hpp"

#include "StarletGraphics/resource/resourceHandle.hpp"

#include <unordered_map>
#include <cstdint>
#include <string>

struct Model;
struct TextureData;

struct MeshGPU;
struct MeshCPU;

class Scene;
class SceneManager;

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager() = default;

	void setBasePath(const std::string& path);

	ResourceHandle addMesh(const std::string& path);
	bool hasMesh(const std::string& path) const;
	bool hasMesh(ResourceHandle handle) const;
	ResourceHandle getMeshHandle(const std::string& path) const;

	ResourceHandle addTexture(const std::string& name, unsigned int textureID);
	bool hasTexture(const std::string& name) const;
	bool hasTexture(ResourceHandle handle) const;
	ResourceHandle getTextureHandle(const std::string& name) const;



	const MeshGPU* getMeshGPU(ResourceHandle handle) const;
	const MeshCPU* getMeshCPU(ResourceHandle handle) const;

	unsigned int getTextureID(ResourceHandle handle) const;

	bool loadMeshes(const std::vector<Model*>& models);
	bool loadTextures(const std::vector<TextureData*>& textures);

	bool processTextureConnections(Scene& scene);
	bool processPrimitives(SceneManager& sm);
	bool processGrids(SceneManager& sm);

private:
	uint32_t nextMeshId = 1;
	std::unordered_map<uint32_t, std::string> meshHandleToPath;
	std::unordered_map<std::string, ResourceHandle> meshPathToHandle;

	uint32_t nextTextureId = 1;
	std::unordered_map<uint32_t, std::string> textureHandleToName;
	std::unordered_map<std::string, ResourceHandle> textureNameToHandle;



	MeshManager meshManager;
	MeshFactory meshFactory;
	TextureManager textureManager;
};