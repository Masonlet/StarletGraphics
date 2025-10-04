#include "StarletGraphics/manager/resourceManager.hpp"

void ResourceManager::setBasePath(const std::string& path) {
  meshManager.setBasePath((path + "/models/").c_str());
  textureManager.setBasePath((path + "/textures/").c_str());
}

ResourceHandle ResourceManager::addMesh(const std::string& path) {
  auto it = meshPathToHandle.find(path);
  if (it != meshPathToHandle.end()) return it->second;

  ResourceHandle handle{ nextMeshId++ };
  meshPathToHandle[path] = handle;
  meshHandleToPath[handle.id] = path;
  return handle;
}
bool ResourceManager::hasMesh(const std::string& path) const {
  return meshPathToHandle.find(path) != meshPathToHandle.end();
}
bool ResourceManager::hasMesh(ResourceHandle handle) const {
  return handle.isValid() && meshHandleToPath.find(handle.id) != meshHandleToPath.end();
}
ResourceHandle ResourceManager::getMeshHandle(const std::string& path) const {
  auto it = meshPathToHandle.find(path);
  return it != meshPathToHandle.end() ? it->second : ResourceHandle{ 0 };
}
const MeshGPU* ResourceManager::getMeshGPU(ResourceHandle handle) const {
  if (!handle.isValid()) return nullptr;

  auto it = meshHandleToPath.find(handle.id);
  if (it == meshHandleToPath.end()) return nullptr;
  const std::string& path = it->second;

  const MeshGPU* gpuData = nullptr;
  return meshManager.getMeshGPU(path, gpuData) ? gpuData : nullptr;
}
const MeshCPU* ResourceManager::getMeshCPU(ResourceHandle handle) const {
  if (!handle.isValid()) return nullptr;
  
  auto it = meshHandleToPath.find(handle.id);
  if (it == meshHandleToPath.end()) return nullptr;
  const std::string& path = it->second;

  const MeshCPU* cpuData = nullptr;
  return meshManager.getMeshCPU(path, cpuData) ? cpuData : nullptr;
}

ResourceHandle ResourceManager::addTexture(const std::string& name, unsigned int textureID) {
  auto it = textureNameToHandle.find(name);
  if (it != textureNameToHandle.end()) return it->second;
 
  ResourceHandle handle{ nextTextureId++ };
  textureHandleToName[handle.id] = name;
  textureNameToHandle[name] = handle;
  return handle;
}
bool ResourceManager::hasTexture(const std::string& name) const {
  return textureNameToHandle.find(name) != textureNameToHandle.end();
}
bool ResourceManager::hasTexture(ResourceHandle handle) const {
  return handle.isValid() && textureHandleToName.find(handle.id) != textureHandleToName.end();
}
ResourceHandle ResourceManager::getTextureHandle(const std::string& name) const {
  auto it = textureNameToHandle.find(name);
  return it != textureNameToHandle.end() ? it->second : ResourceHandle{ 0 };
}
unsigned int ResourceManager::getTextureID(ResourceHandle handle) const {
  if (!handle.isValid()) return 0;

  auto it = textureHandleToName.find(handle.id);
  if (it == textureHandleToName.end()) return 0;

  const std::string& textureName = it->second;
  return textureManager.getTextureID(textureName);
}