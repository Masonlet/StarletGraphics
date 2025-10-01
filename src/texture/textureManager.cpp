#include "StarletGraphics/texture/texture.hpp"

#include "StarletGraphics/texture/textureManager.hpp"
#include "StarletParser/utils/log.hpp"

TextureManager::~TextureManager() {
  for (std::map<std::string, TextureGPU>::iterator it = nameToGPUTextures.begin(); it != nameToGPUTextures.end(); ++it)
    loader.unloadTexture(it->second);
}

unsigned int TextureManager::getTextureID(const std::string& name) const {
  std::map<std::string, TextureGPU>::const_iterator it = nameToGPUTextures.find(name);
  return (it == nameToGPUTextures.end()) ? 0u : it->second.id;
}

bool TextureManager::addTexture(const std::string& name, const std::string& filePath) {
  if (exists(name)) return true;

  TextureCPU cpuTexture;
  if (!loader.loadTexture2D(basePath + filePath, cpuTexture))
    return error("TextureManager", "addTexture", "Failed load: " + basePath + filePath);

  TextureGPU gpuTexture;
  if (!loader.uploadTexture2D(cpuTexture, gpuTexture, true))
    return error("TextureManager", "addTexture", "Failed upload: " + name);

  cpuTexture.freePixels();
  nameToGPUTextures[name] = std::move(gpuTexture);
  return debugLog("TextureManager", "addTexture", "Added texture: " + name + "at: " + filePath);
}

bool TextureManager::addTextureCube(const std::string& name, const std::string(&facePaths)[6]) {
  if (exists(name)) return true;

  std::string fullPaths[6];
  for (int i = 0; i < 6; ++i) fullPaths[i] = basePath + facePaths[i];

  TextureCPU faces[6];
  if (!loader.loadCubeFaces(fullPaths, faces))
    return error("TextureManager", "addCubeTexture", "Failed to load: " + name);

  TextureGPU cube;
  if (!loader.uploadTextureCube(faces, cube, true))
    return error("TextureManager", "addCubeTexture", "Failed to upload: " + name);

  for (TextureCPU& face : faces) face.freePixels();
  
  nameToGPUTextures[name] = std::move(cube);
  return debugLog("TextureManager", "addTextureCube", "Added texture cube: " + name);
}
