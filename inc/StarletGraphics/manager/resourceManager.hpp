#pragma once

#include "StarletGraphics/manager/meshManager.hpp"
#include "StarletGraphics/manager/textureManager.hpp"

#include "StarletGraphics/resource/resourceHandle.hpp"

#include <unordered_map>
#include <cstdint>
#include <string>

struct MeshGPU;
struct MeshCPU;

struct TextureGPU;
struct TextureCPU;

class ResourceManager {
public:
	ResourceManager() = default;
	~ResourceManager() = default;

	void setBasePath(const std::string& path);

	MeshManager& getMeshManager() { return meshManager; }
	TextureManager& getTextureManager() { return textureManager; }

	ResourceHandle addMesh(const std::string& path);
	bool hasMesh(const std::string& path) const;
	bool hasMesh(ResourceHandle handle) const;
	ResourceHandle getMeshHandle(const std::string& path) const;
	const MeshGPU* getMeshGPU(ResourceHandle handle) const;
	const MeshCPU* getMeshCPU(ResourceHandle handle) const;

	ResourceHandle addTexture(const std::string& name, unsigned int textureID);
	bool hasTexture(const std::string& name) const;
	bool hasTexture(ResourceHandle handle) const;
	ResourceHandle getTextureHandle(const std::string& name) const;
	unsigned int getTextureID(ResourceHandle handle) const;

private:
	MeshManager meshManager;
	uint32_t nextMeshId = 1;
	std::unordered_map<uint32_t, std::string> meshHandleToPath;
	std::unordered_map<std::string, ResourceHandle> meshPathToHandle;

	TextureManager textureManager;
	uint32_t nextTextureId = 1;
	std::unordered_map<uint32_t, std::string> textureHandleToName;
	std::unordered_map<std::string, ResourceHandle> textureNameToHandle;
};