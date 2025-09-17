#pragma once

struct Shader;
#include <string>

struct ShaderLoader {
	bool loadSource(std::string& outSource, const std::string& path);

	bool linkProgram(unsigned int& outProgramID, unsigned int vertID, unsigned int fragID);
	bool compileShader(unsigned int& outShaderID, int glShaderType, const std::string& source);

	void unloadShader(Shader& shader);
};