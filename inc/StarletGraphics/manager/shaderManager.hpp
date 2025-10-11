#pragma once

#include "StarletGraphics/manager/manager.hpp"

#include "StarletGraphics/resource/shaderGPU.hpp"
#include "StarletGraphics/handler/shaderHandler.hpp"
#include "StarletSerializer/parser.hpp"
#include <map>

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
	Parser parser;
	ShaderHandler handler;
	std::map<std::string, ShaderGPU> nameToShaders;
};
