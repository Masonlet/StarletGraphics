#pragma once

struct Shader;
#include <string>

struct ShaderLoader {
	void unloadShader(Shader& shader);
	bool createProgramFromPaths(Shader& out, const std::string& vertPath, const std::string& fragPath);

private:
	bool compileShader(unsigned int& outShaderID, int glShaderType, const std::string& source);
	bool linkProgram(unsigned int& outProgramID, unsigned int vertID, unsigned int fragID);
};