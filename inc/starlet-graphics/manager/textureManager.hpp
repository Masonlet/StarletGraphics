#pragma once

#include "starlet-graphics/manager/manager.hpp"
#include "starlet-graphics/resource/textureGPU.hpp"

#include "starlet-serializer/parser/imageParser.hpp"
#include "starlet-graphics/handler/textureHandler.hpp"

#include <map>

namespace Starlet::Graphics {
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
		Serializer::ImageParser parser;
		TextureHandler handler;
		std::map<std::string, TextureGPU> nameToGPUTextures;
	};
}