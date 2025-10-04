#pragma once

#include "StarletGraphics/handler/resourceHandler.hpp"

struct TextureCPU;
struct TextureGPU;

struct TextureHandler : public ResourceHandler<TextureCPU, TextureGPU> {
	bool upload(TextureCPU& cpu, TextureGPU& gpu) override;
	bool upload(TextureCPU& cpu, TextureGPU& gpu, bool generateMIPMap);

	bool upload(TextureCPU(&faces)[6], TextureGPU& cubeOut);
	bool upload(TextureCPU(&faces)[6], TextureGPU& cubeOut, bool generateMIPMap);

	void unload(TextureGPU& gpu) override;
};