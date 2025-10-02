#pragma once

#include "StarletGraphics/manager/manager.hpp"
#include "StarletGraphics/resource/shader.hpp"
#include "StarletGraphics/handler/shaderHandler.hpp"
#include <map>

class ShaderManager : public Manager {
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
	ShaderHandler handler;
	std::map<std::string, Shader> nameToShaders;
};
