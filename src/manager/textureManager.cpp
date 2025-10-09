#include "StarletGraphics/manager/textureManager.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletGraphics/resource/textureCPU.hpp"

TextureManager::~TextureManager() {
  for (std::map<std::string, TextureGPU>::iterator it = nameToGPUTextures.begin(); it != nameToGPUTextures.end(); ++it)
    handler.unload(it->second);
}

unsigned int TextureManager::getTextureID(const std::string& name) const {
  std::map<std::string, TextureGPU>::const_iterator it = nameToGPUTextures.find(name);
  return (it == nameToGPUTextures.end()) ? 0u : it->second.id;
}

bool TextureManager::addTexture(const std::string& name, const std::string& path) {
  if (exists(name)) return true;

  TextureCPU cpuTexture;
  if (!parser.parseBMP(basePath + path, cpuTexture))
    return error("TextureManager", "addTexture", "Failed load: " + basePath + path);

  TextureGPU gpuTexture;
  if (!handler.upload(cpuTexture, gpuTexture, true))
    return error("TextureManager", "addTexture", "Failed upload: " + name);

  nameToGPUTextures[name] = std::move(gpuTexture);
  return debugLog("TextureManager", "addTexture", "Added texture: " + name + " at: " + path);
}

bool TextureManager::addTextureCube(const std::string& name, const std::string(&facePaths)[6]) {
  if (exists(name)) return true;

  TextureCPU faces[6];
  for (int i = 0; i < 6; ++i)
    if (!parser.parseBMP(basePath + facePaths[i], faces[i]))
      return error("TextureLoader", "loadCubeFaces", "Failed to add");

  TextureGPU cube;
  if (!handler.upload(faces, cube, true))
    return error("TextureManager", "addCubeTexture", "Failed to upload: " + name);

  nameToGPUTextures[name] = std::move(cube);
  return debugLog("TextureManager", "addTextureCube", "Added texture cube: " + name);
}
