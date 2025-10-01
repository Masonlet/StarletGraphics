#pragma once


#include "StarletGraphics/resourceManager.hpp"

#include "texture.hpp"
#include "textureLoader.hpp"
#include <map>

class TextureManager : public ResourceManager {
public:
	~TextureManager();

	bool exists(const std::string& name) const override {
		return nameToGPUTextures.find(name) != nameToGPUTextures.end();
	}

	bool addTexture(const std::string& name, const std::string& filePath);
	bool addTextureCube(const std::string& name, const std::string(&facePaths)[6]);

	unsigned int getTextureID(const std::string& name) const;

private:
	TextureLoader loader;
	std::map<std::string, TextureGPU> nameToGPUTextures;
};