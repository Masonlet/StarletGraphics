#include "StarletGraphics/texture.hpp"
#include "StarletGraphics/textureManager.hpp"
#include "StarletParsers/utils/log.hpp"

TextureManager::~TextureManager() {
  for (std::map<std::string, Texture>::iterator it = nameToTextures.begin(); it != nameToTextures.end(); ++it)
    loader.unloadTexture(it->second);
}
bool TextureManager::findTexture(const std::string& name) const {
  return nameToTextures.find(name) != nameToTextures.end();
}
bool TextureManager::getTexture(const std::string& name, Texture*& data) {
  std::map<std::string, Texture>::iterator it = nameToTextures.find(name);
  if (it == nameToTextures.end()) return error("TextureManager", "getTexture", "Texture not found: " + name);
  data = &it->second;
  return true;
}
unsigned int TextureManager::getTextureID(const std::string& name) const {
  std::map<std::string, Texture>::const_iterator it = nameToTextures.find(name);
  return (it == nameToTextures.end()) ? 0u : it->second.id;
}

bool TextureManager::addTexture(const std::string& name, const std::string& filePath) {
  if (findTexture(name)) return true;

  Texture texture;
  if (!loader.loadTexture2D(basePath + "/textures/" + filePath, texture))
    return error("TextureManager", "addTexture", "Failed load: " + basePath + "/textures/" + filePath);

  if (!loader.uploadTexture2D(texture, true))
    return error("TextureManager", "addTexture", "Failed upload: " + name);

  texture.freePixels();
  nameToTextures[name] = std::move(texture);
  return true;
}

bool TextureManager::addCubeTexture(const std::string& name, const std::string(&facePaths)[6]) {
  if (findTexture(name)) return true;

  std::string fullPaths[6];
  for (int i = 0; i < 6; ++i) fullPaths[i] = basePath + "/textures/" + facePaths[i];

  Texture faces[6];
  if (!loader.loadCubeFaces(fullPaths, faces))
    return error("TextureManager", "addCubeTexture", "Failed to load: " + name);

  Texture cube;
  if (!loader.uploadTextureCube(faces, cube, true))
    return error("TextureManager", "addCubeTexture", "Failed to upload: " + name);

  for (int i = 0; i < 6; ++i) faces[i].freePixels();
  nameToTextures[name] = std::move(cube);
  return true;
}