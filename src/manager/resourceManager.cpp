#include "StarletGraphics/manager/resourceManager.hpp"

MeshHandle ResourceManager::addMesh(const std::string& path, const MeshGPU* gpu, const MeshCPU* cpu) {
  auto it = meshPathToHandle.find(path);
  if (it != meshPathToHandle.end()) return it->second;

  MeshHandle handle{ nextMeshId++ };
  meshGPULookup[handle.id] = gpu;
  meshCPULookup[handle.id] = cpu;
  meshPathToHandle[path] = handle;
  return handle;
}
bool ResourceManager::hasMesh(const std::string& path) const {
  return meshPathToHandle.find(path) != meshPathToHandle.end();
}
bool ResourceManager::hasMesh(MeshHandle handle) const {
  return handle.isValid() && meshGPULookup.find(handle.id) != meshGPULookup.end();
}
MeshHandle ResourceManager::getMeshHandle(const std::string& path) const {
  auto it = meshPathToHandle.find(path);
  return it != meshPathToHandle.end() ? it->second : MeshHandle{ 0 };
}
const MeshGPU* ResourceManager::getMeshGPU(MeshHandle handle) const {
  if (!handle.isValid()) return nullptr;
  auto it = meshGPULookup.find(handle.id);
  return it != meshGPULookup.end() ? it->second : nullptr;
}
const MeshCPU* ResourceManager::getMeshCPU(MeshHandle handle) const {
  if (!handle.isValid()) return nullptr;
  auto it = meshCPULookup.find(handle.id);
  return it != meshCPULookup.end() ? it->second : nullptr;
}

TextureHandle ResourceManager::addTexture(const std::string& name, unsigned int textureID) {
  auto it = textureNameToHandle.find(name);
  if (it != textureNameToHandle.end()) return it->second;
 
  TextureHandle handle{ nextTextureId++ };
  textureGLIDLookup[handle.id] = textureID;
  textureNameToHandle[name] = handle;
  return handle;
}
bool ResourceManager::hasTexture(const std::string& name) const {
  return textureNameToHandle.find(name) != textureNameToHandle.end();
}
bool ResourceManager::hasTexture(TextureHandle handle) const {
  return handle.isValid() && textureGLIDLookup.find(handle.id) != textureGLIDLookup.end();
}
TextureHandle ResourceManager::getTextureHandle(const std::string& name) const {
  auto it = textureNameToHandle.find(name);
  return it != textureNameToHandle.end() ? it->second : TextureHandle{ 0 };
}
unsigned int ResourceManager::getTextureID(TextureHandle handle) const {
  if (!handle.isValid()) return 0;
  auto it = textureGLIDLookup.find(handle.id);
  return it != textureGLIDLookup.end() ? it->second : 0;
}