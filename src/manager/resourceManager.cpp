#include "StarletGraphics/manager/resourceManager.hpp"

void ResourceManager::setBasePath(const std::string& path) {
  meshManager.setBasePath((path + "/models/").c_str());
  textureManager.setBasePath((path + "/textures/").c_str());
}

MeshHandle ResourceManager::addMesh(const std::string& path) {
  auto it = meshPathToHandle.find(path);
  if (it != meshPathToHandle.end()) return it->second;

  MeshHandle handle{ nextMeshId++ };
  meshPathToHandle[path] = handle;
  meshHandleToPath[handle.id] = path;
  return handle;
}
bool ResourceManager::hasMesh(const std::string& path) const {
  return meshPathToHandle.find(path) != meshPathToHandle.end();
}
bool ResourceManager::hasMesh(MeshHandle handle) const {
  return handle.isValid() && meshHandleToPath.find(handle.id) != meshHandleToPath.end();
}
MeshHandle ResourceManager::getMeshHandle(const std::string& path) const {
  auto it = meshPathToHandle.find(path);
  return it != meshPathToHandle.end() ? it->second : MeshHandle{ 0 };
}
const MeshGPU* ResourceManager::getMeshGPU(MeshHandle handle) const {
  if (!handle.isValid()) return nullptr;

  auto it = meshHandleToPath.find(handle.id);
  if (it == meshHandleToPath.end()) return nullptr;
  const std::string& path = it->second;

  const MeshGPU* gpuData = nullptr;
  return meshManager.getMeshGPU(path, gpuData) ? gpuData : nullptr;
}
const MeshCPU* ResourceManager::getMeshCPU(MeshHandle handle) const {
  if (!handle.isValid()) return nullptr;
  
  auto it = meshHandleToPath.find(handle.id);
  if (it == meshHandleToPath.end()) return nullptr;
  const std::string& path = it->second;

  const MeshCPU* cpuData = nullptr;
  return meshManager.getMeshCPU(path, cpuData) ? cpuData : nullptr;
}

TextureHandle ResourceManager::addTexture(const std::string& name, unsigned int textureID) {
  auto it = textureNameToHandle.find(name);
  if (it != textureNameToHandle.end()) return it->second;
 
  TextureHandle handle{ nextTextureId++ };
  textureHandleToName[handle.id] = name;
  textureNameToHandle[name] = handle;
  return handle;
}
bool ResourceManager::hasTexture(const std::string& name) const {
  return textureNameToHandle.find(name) != textureNameToHandle.end();
}
bool ResourceManager::hasTexture(TextureHandle handle) const {
  return handle.isValid() && textureHandleToName.find(handle.id) != textureHandleToName.end();
}
TextureHandle ResourceManager::getTextureHandle(const std::string& name) const {
  auto it = textureNameToHandle.find(name);
  return it != textureNameToHandle.end() ? it->second : TextureHandle{ 0 };
}
unsigned int ResourceManager::getTextureID(TextureHandle handle) const {
  if (!handle.isValid()) return 0;

  auto it = textureHandleToName.find(handle.id);
  if (it == textureHandleToName.end()) return 0;

  const std::string& textureName = it->second;
  return textureManager.getTextureID(textureName);
}