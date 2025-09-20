#pragma once

#include "texture.hpp"
#include "textureLoader.hpp"
#include <string>
#include <map>

class TextureManager {
public:
	TextureManager() = default;
	~TextureManager();

	void setBasePath(const std::string& path) { basePath = path; }

	bool addTexture(const std::string& name, const std::string& filePath);
	bool addTextureCube(const std::string& name, const std::string(&facePaths)[6]);

	bool findTexture(const std::string& name) const;
	unsigned int getTextureID(const std::string& name) const;

private:
	TextureLoader loader;
	std::string basePath;
	std::map<std::string, TextureGPU> nameToGPUTextures;
};