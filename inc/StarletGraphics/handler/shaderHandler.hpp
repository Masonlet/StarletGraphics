#pragma once

struct Shader;
#include <string>

struct ShaderHandler {
	bool linkProgram(unsigned int& outProgramID, unsigned int vertID, unsigned int fragID);
	bool compileShader(unsigned int& outShaderID, int glShaderType, const std::string& source);
	void unloadShader(Shader& shader);
};