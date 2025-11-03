#include "StarletGraphics/manager/textureManager.hpp"
#include "StarletLogger/logger.hpp"

#include "StarletSerializer/data/bmpData.hpp"
#include "StarletGraphics/resource/textureCPU.hpp"

namespace Starlet::Graphics {
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

    Serializer::BmpData bmpData;
    if (!parser.parse(basePath + path, bmpData))
      return Logger::error("TextureManager", "addTexture", "Failed load: " + basePath + path);

    TextureCPU cpuTexture;
    cpuTexture.width = bmpData.width;
    cpuTexture.height = bmpData.height;
    cpuTexture.pixelSize = bmpData.pixelSize;
		cpuTexture.byteSize = bmpData.byteSize;
    cpuTexture.pixels = std::move(bmpData.pixels);

    TextureGPU gpuTexture;
    if (!handler.upload(cpuTexture, gpuTexture, true))
      return Logger::error("TextureManager", "addTexture", "Failed upload: " + name);

    nameToGPUTextures[name] = std::move(gpuTexture);
    return Logger::debugLog("TextureManager", "addTexture", "Added texture: " + name + " at: " + path);
  }

  bool TextureManager::addTextureCube(const std::string& name, const std::string(&facePaths)[6]) {
    if (exists(name)) return true;

    TextureCPU faces[6];
    for (int i = 0; i < 6; ++i) {
      Serializer::BmpData bmpData;
      if (!parser.parse(basePath + facePaths[i], bmpData))
        return Logger::error("TextureManager", "addTextureCube", "Failed to load face " + std::to_string(i));

      faces[i].width = bmpData.width;
      faces[i].height = bmpData.height;
      faces[i].pixelSize = bmpData.pixelSize;
      faces[i].byteSize = bmpData.byteSize;
      faces[i].pixels = std::move(bmpData.pixels);
    }

    TextureGPU cube;
    if (!handler.upload(faces, cube, true))
      return Logger::error("TextureManager", "addCubeTexture", "Failed to upload: " + name);

    nameToGPUTextures[name] = std::move(cube);
    return Logger::debugLog("TextureManager", "addTextureCube", "Added texture cube: " + name);
  }
}