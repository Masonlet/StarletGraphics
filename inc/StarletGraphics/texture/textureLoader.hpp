#pragma once

struct TextureCPU;
struct TextureGPU;
#include <string>

struct TextureLoader {
	bool loadTexture2D(const std::string& path, TextureCPU& outTexture);
	bool loadCubeFaces(const std::string(&facePaths)[6], TextureCPU(&facesOut)[6]);

	bool uploadTexture2D(TextureCPU& cpuTexture, TextureGPU& gpuTexture, bool generateMIPMap);
	bool uploadTextureCube(TextureCPU(&faces)[6], TextureGPU& cubeOut, bool generateMIPMap);

	void unloadTexture(TextureGPU& texture);
};