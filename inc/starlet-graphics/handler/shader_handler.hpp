#pragma once

#include "starlet-graphics/handler/resource_handler.hpp"

#include <string>

namespace Starlet::Graphics {
	struct ShaderCPU;
	struct ShaderGPU;

	struct ShaderHandler : public ResourceHandler<ShaderCPU, ShaderGPU> {
		bool upload(ShaderCPU& cpu, ShaderGPU& gpu) override;
		void unload(ShaderGPU& shader) override;

	private:
		bool linkProgram(unsigned int& outProgramID, unsigned int vertID, unsigned int fragID);
		bool compileShader(unsigned int& outShaderID, int glShaderType, const std::string& source);
	};
}