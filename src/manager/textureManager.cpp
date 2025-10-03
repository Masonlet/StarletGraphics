#include "StarletGraphics/manager/textureManager.hpp"
#include "StarletGraphics/resource/textureCPU.hpp"
#include "StarletGraphics/resource/textureGPU.hpp"

#include "StarletParser/parser.hpp"
#include "StarletParser/utils/log.hpp"

TextureManager::~TextureManager() {
  for (std::map<std::string, TextureGPU>::iterator it = nameToGPUTextures.begin(); it != nameToGPUTextures.end(); ++it)
    handler.unloadTexture(it->second);
}

unsigned int TextureManager::getTextureID(const std::string& name) const {
  std::map<std::string, TextureGPU>::const_iterator it = nameToGPUTextures.find(name);
  return (it == nameToGPUTextures.end()) ? 0u : it->second.id;
}

bool TextureManager::addTexture(const std::string& name, const std::string& filePath) {
  if (exists(name)) return true;

  Parser parser;
  TextureCPU cpuTexture;
  if (!parser.parseBMP(basePath + filePath, cpuTexture))
    return error("TextureManager", "addTexture", "Failed load: " + basePath + filePath);

  TextureGPU gpuTexture;
  if (!handler.uploadTexture2D(cpuTexture, gpuTexture, true))
    return error("TextureManager", "addTexture", "Failed upload: " + name);

  cpuTexture.freePixels();
  nameToGPUTextures[name] = std::move(gpuTexture);
  return debugLog("TextureManager", "addTexture", "Added texture: " + name + " at: " + filePath);
}

bool TextureManager::addTextureCube(const std::string& name, const std::string(&facePaths)[6]) {
  if (exists(name)) return true;

  std::string fullPaths[6];
  for (int i = 0; i < 6; ++i) fullPaths[i] = basePath + facePaths[i];

  Parser parser;
  TextureCPU faces[6];
  for (int i = 0; i < 6; ++i)
    if (!parser.parseBMP((fullPaths[i]).c_str(), faces[i]))
      return error("TextureLoader", "loadCubeFaces", "Failed face[" + std::to_string(i) + "]: " + fullPaths[i]);

  TextureGPU cube;
  if (!handler.uploadTextureCube(faces, cube, true))
    return error("TextureManager", "addCubeTexture", "Failed to upload: " + name);

  for (TextureCPU& face : faces) face.freePixels();
  
  nameToGPUTextures[name] = std::move(cube);
  return debugLog("TextureManager", "addTextureCube", "Added texture cube: " + name);
}
