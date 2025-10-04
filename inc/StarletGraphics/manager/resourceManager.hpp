#pragma once

#include "StarletGraphics/manager/meshManager.hpp"
#include "StarletGraphics/manager/textureManager.hpp"

#include "StarletGraphics/resource/meshHandle.hpp"
#include "StarletGraphics/resource/textureHandle.hpp"

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

	MeshHandle addMesh(const std::string& path);
	bool hasMesh(const std::string& path) const;
	bool hasMesh(MeshHandle handle) const;
	MeshHandle getMeshHandle(const std::string& path) const;
	const MeshGPU* getMeshGPU(MeshHandle handle) const;
	const MeshCPU* getMeshCPU(MeshHandle handle) const;

	TextureHandle addTexture(const std::string& name, unsigned int textureID);
	bool hasTexture(const std::string& name) const;
	bool hasTexture(TextureHandle handle) const;
	TextureHandle getTextureHandle(const std::string& name) const;
	unsigned int getTextureID(TextureHandle handle) const;

private:
	MeshManager meshManager;
	uint32_t nextMeshId = 1;
	std::unordered_map<uint32_t, std::string> meshHandleToPath;
	std::unordered_map<std::string, MeshHandle> meshPathToHandle;

	TextureManager textureManager;
	uint32_t nextTextureId = 1;
	std::unordered_map<uint32_t, std::string> textureHandleToName;
	std::unordered_map<std::string, TextureHandle> textureNameToHandle;
};