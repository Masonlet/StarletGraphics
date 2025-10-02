#pragma once

struct TextureCPU;
struct TextureGPU;
#include <string>

struct TextureHandler {
	bool uploadTexture2D(TextureCPU& cpuTexture, TextureGPU& gpuTexture, bool generateMIPMap);
	bool uploadTextureCube(TextureCPU(&faces)[6], TextureGPU& cubeOut, bool generateMIPMap);
	void unloadTexture(TextureGPU& texture);
};