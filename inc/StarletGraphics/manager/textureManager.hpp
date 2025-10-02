#pragma once

#include "StarletGraphics/manager/manager.hpp"

#include "StarletGraphics/resource/textureGPU.hpp"
#include "StarletGraphics/handler/textureHandler.hpp"

#include <map>

class TextureManager : public Manager {
public:
	~TextureManager();

	bool exists(const std::string& name) const override {
		return nameToGPUTextures.find(name) != nameToGPUTextures.end();
	}

	bool addTexture(const std::string& name, const std::string& filePath);
	bool addTextureCube(const std::string& name, const std::string(&facePaths)[6]);

	unsigned int getTextureID(const std::string& name) const;

private:
	TextureHandler handler;
	std::map<std::string, TextureGPU> nameToGPUTextures;
};