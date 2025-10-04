#pragma once

#include "StarletParser/parser.hpp"
#include "StarletGraphics/handler/textureHandler.hpp"

#include <string>

class TextureLoader {
public:
	bool addTexture2D(TextureCPU& cpuTexture, const std::string& path);
	bool addTextureCube(TextureCPU(&faces)[6], const std::string(&path)[6]);

	bool uploadTexture2D(TextureCPU& cpuTexture, TextureGPU& gpuTexture, bool generateMIPMap);
	bool uploadTextureCube(TextureCPU(&faces)[6], TextureGPU& gpuTexture, bool generateMIPMap);
	void unloadTexture(TextureGPU& texture);

private:
	Parser parser;
	TextureHandler handler;
};