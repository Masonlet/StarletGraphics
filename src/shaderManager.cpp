#include "starletgraphics/shaderManager.hpp"
#include "starletparsers/utils/log.hpp"
#include <glad/glad.h>
#include <sstream>

ShaderManager::~ShaderManager() {
	for (std::map<std::string, Shader>::iterator it = nameToShaders.begin(); it != nameToShaders.end(); ++it)
		loader.unloadShader(it->second);
}

bool ShaderManager::useProgram(const std::string& name) const {
	std::map<std::string, Shader>::const_iterator it = nameToShaders.find(name);
	if (it == nameToShaders.end()) 
		return error("ShaderManager", "useProgram", "Shader not found: " + name);
	if (!it->second.linked || it->second.programID == 0) 
		return error("ShaderManager", "useProgram", "Shader not linked, or has invalid id: " + name);

	glUseProgram(it->second.programID);
	return true;
}
bool ShaderManager::createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath) {
	Shader shader{};
	if (!loader.createProgramFromPaths(shader, basePath + '/' + vertPath, basePath + '/' + fragPath))
		return error("ShaderManager", "createProgramFromPaths", "Failed creating program for: " + name);

	std::map<std::string, Shader>::iterator it = nameToShaders.find(name);
	if (it != nameToShaders.end()) 
		loader.unloadShader(it->second);

	nameToShaders[name] = std::move(shader);
	return true;
}

unsigned int ShaderManager::getProgramID(const std::string& name) const {
	std::map<std::string, Shader>::const_iterator it = nameToShaders.find(name);
	return (it == nameToShaders.end()) ? 0u : it->second.programID;
}

bool ShaderManager::findShader(const std::string& name) const {
	return nameToShaders.find(name) != nameToShaders.end();
}
bool ShaderManager::getShader(const std::string& name, Shader*& dataOut) {
	std::map<std::string, Shader>::iterator it = nameToShaders.find(name);
	if (it == nameToShaders.end()) return error("ShaderManager", "getShader", "Shader not found: " + name);
	dataOut = &it->second;
	return true;
}
bool ShaderManager::getShader(const std::string& name, const Shader*& dataOut) const {
	std::map<std::string, Shader>::const_iterator it = nameToShaders.find(name);
	if (it == nameToShaders.end()) return error("ShaderManager", "getShader", "Shader not found: " + name);
	dataOut = &it->second;
	return true;
}