#include "StarletGraphics/loader/textureLoader.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletGraphics/resource/textureCPU.hpp"
#include "StarletGraphics/resource/textureGPU.hpp"

bool TextureLoader::addTexture2D(TextureCPU& cpuTexture, const std::string& path) {
	if (!parser.parseBMP(path, cpuTexture))
		return error("TextureManager", "addTexture2D", "Failed load: " + path);

	return true;
}
bool TextureLoader::addTextureCube(TextureCPU(&faces)[6], const std::string(& path)[6]) {
	for (int i = 0; i < 6; ++i)
		if (!parser.parseBMP(path[i], faces[i]))
			return error("TextureLoader", "addTextureCube", "Failed face[" + std::to_string(i) + "]: " + path[i]);

	return true;
}

bool TextureLoader::uploadTexture2D(TextureCPU& cpuTexture, TextureGPU& gpuTexture, bool generateMIPMap) {
	if (!handler.upload(cpuTexture, gpuTexture, generateMIPMap))
		return error("TextureManager", "uploadTexture2D", "Failed upload");

	return true;
}

bool TextureLoader::uploadTextureCube(TextureCPU(&faces)[6], TextureGPU& gpuTexture, bool generateMIPMap) {
	if (!handler.upload(faces, gpuTexture, generateMIPMap))
		return error("TextureManager", "uploadTextureCube", "Failed to upload");

	return true;
}

void TextureLoader::unloadTexture(TextureGPU& texture) {
	handler.unload(texture);
}