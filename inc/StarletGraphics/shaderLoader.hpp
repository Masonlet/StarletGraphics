#pragma once

struct Shader;
#include <string>

struct ShaderLoader {
	void unloadShader(Shader& shader);
	bool loadAndCompileShader(unsigned int& outShaderID, int glShaderType, const std::string& path);
	bool linkProgram(unsigned int& outProgramID, unsigned int vertID, unsigned int fragID);

private:
	bool compileShader(unsigned int& outShaderID, int glShaderType, const std::string& source);
};