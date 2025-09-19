#pragma once

#include "shader.hpp"
#include "shaderLoader.hpp"
#include <vector>
#include <map>
#include <string>

class ShaderManager {
public:
	ShaderManager() = default;
	~ShaderManager();

	void setBasePath(const char* path) { basePath = path; }

	bool useProgram(const std::string& name) const;
	bool createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath);

	bool findShader(const std::string& name) const;
	bool getShader(const std::string& name, Shader*& dataOut);
	bool getShader(const std::string& name, const Shader*& dataOut) const;
	unsigned int getProgramID(const std::string& name) const;

private:
	ShaderLoader loader;
	std::string basePath;
	std::map<std::string, Shader> nameToShaders;
};
