#include "StarletGraphics/loader/textureLoader.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletGraphics/resource/textureCPU.hpp"
#include "StarletGraphics/resource/textureGPU.hpp"

bool TextureLoader::addTexture2D(TextureCPU& cpuTexture, const std::string& path) {
	if (!parser.parseBMP(path, cpuTexture))
		return error("TextureManager", "addTexture", "Failed load: " + path);

	return true;
}
bool TextureLoader::addTextureCube(TextureCPU(&faces)[6], const std::string(& path)[6]) {
	for (int i = 0; i < 6; ++i)
		if (!parser.parseBMP(path[i], faces[i]))
			return error("TextureLoader", "loadCubeFaces", "Failed face[" + std::to_string(i) + "]: " + path[i]);

	return true;
}

bool TextureLoader::uploadTexture2D(TextureCPU& cpuTexture, TextureGPU& gpuTexture, bool generateMIPMap) {
	if (!handler.uploadTexture2D(cpuTexture, gpuTexture, true))
		return error("TextureManager", "addTexture", "Failed upload");

	return true;
}

bool TextureLoader::uploadTextureCube(TextureCPU(&faces)[6], TextureGPU& gpuTexture, bool generateMIPMap) {
	if (!handler.uploadTextureCube(faces, gpuTexture, true))
		return error("TextureManager", "addCubeTexture", "Failed to upload");

	return true;
}

void TextureLoader::unloadTexture(TextureGPU& texture) {
	handler.unloadTexture(texture);
}