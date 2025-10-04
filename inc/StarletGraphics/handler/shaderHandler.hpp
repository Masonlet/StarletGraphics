#pragma once

#include "StarletGraphics/handler/resourceHandler.hpp"

#include <string>

struct ShaderCPU;
struct ShaderGPU;

struct ShaderHandler : public ResourceHandler<ShaderCPU, ShaderGPU> {
	bool upload(ShaderCPU& cpu, ShaderGPU& gpu) override;
	void unload(ShaderGPU& shader) override;

private:
	bool linkProgram(unsigned int& outProgramID, unsigned int vertID, unsigned int fragID);
	bool compileShader(unsigned int& outShaderID, int glShaderType, const std::string& source);
};