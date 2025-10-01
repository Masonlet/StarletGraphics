#pragma once

#include "StarletGraphics/resourceManager.hpp"

#include "shader.hpp"
#include "shaderLoader.hpp"
#include <map>

class ShaderManager : public ResourceManager {
public:
	~ShaderManager();

	bool useProgram(const std::string& name) const;

	bool exists(const std::string& name) const {
		return nameToShaders.find(name) != nameToShaders.end();
	}

	bool createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath);

	bool getShader(const std::string& name, Shader*& dataOut);
	bool getShader(const std::string& name, const Shader*& dataOut) const;
	unsigned int getProgramID(const std::string& name) const;

private:
	ShaderLoader loader;
	std::map<std::string, Shader> nameToShaders;
};
