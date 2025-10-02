#pragma once

#include "StarletGraphics/resource/meshCPU.hpp"
#include "StarletGraphics/resource/meshGPU.hpp"
#include "StarletGraphics/resource/meshHandle.hpp"
#include "StarletGraphics/resource/textureCPU.hpp"
#include "StarletGraphics/resource/textureGPU.hpp"
#include "StarletGraphics/resource/textureHandle.hpp"

#include <unordered_map>
#include <cstdint>
#include <string>

class ResourceManager {
public:
	ResourceManager() = default;
	~ResourceManager() = default;

	MeshHandle addMesh(const std::string& path, const MeshGPU* gpu, const MeshCPU* cpu);
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
	uint32_t nextMeshId = 1;
	std::unordered_map<uint32_t, const MeshGPU*> meshGPULookup;
	std::unordered_map<uint32_t, const MeshCPU*> meshCPULookup;
	std::unordered_map<std::string, MeshHandle> meshPathToHandle;

	uint32_t nextTextureId = 1;  
	std::unordered_map<uint32_t, unsigned int> textureGLIDLookup;
	std::unordered_map<std::string, TextureHandle> textureNameToHandle;
};