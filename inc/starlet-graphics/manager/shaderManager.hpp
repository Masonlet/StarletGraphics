#pragma once

#include "starlet-graphics/manager/manager.hpp"

#include "starlet-graphics/resource/shaderGPU.hpp"
#include "starlet-graphics/handler/shaderHandler.hpp"
#include "starlet-serializer/parser/parser.hpp"
#include <map>

namespace Starlet::Graphics {
	class ShaderManager : public Manager {
	public:
		~ShaderManager();

		bool exists(const std::string& name) const {
			return nameToShaders.find(name) != nameToShaders.end();
		}

		bool createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath);

		bool getShader(const std::string& name, ShaderGPU*& dataOut);
		bool getShader(const std::string& name, const ShaderGPU*& dataOut) const;
		unsigned int getProgramID(const std::string& name) const;

	private:
		Serializer::Parser parser;
		ShaderHandler handler;
		std::map<std::string, ShaderGPU> nameToShaders;
	};
}