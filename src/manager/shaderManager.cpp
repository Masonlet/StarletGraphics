#include "StarletGraphics/manager/shaderManager.hpp"
#include "StarletParser/utils/log.hpp"
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
	if (exists(name)) {
		std::map<std::string, Shader>::iterator it = nameToShaders.find(name);
		loader.unloadShader(it->second);
		nameToShaders.erase(it);
	}

	Shader shader{};

	std::string vertexSource, fragmentSource;
	if (!loader.loadSource(vertexSource, basePath + vertPath))
		return error("ShaderLoader", "createProgramFromPaths", "Failed to load vertex shader source");

	if (!loader.loadSource(fragmentSource, basePath + fragPath))
		return error("ShaderLoader", "createProgramFromPaths", "Failed to load fragment shader source");

	if (!loader.compileShader(shader.vertexID, GL_VERTEX_SHADER, vertexSource))
		return error("ShaderLoader", "createProgramFromPaths", "Failed to compile vertex shader");

	if (!loader.compileShader(shader.fragmentID, GL_FRAGMENT_SHADER, fragmentSource)) {
		loader.unloadShader(shader);
		return error("ShaderLoader", "createProgramFromPaths", "Failed to compile fragment shader");
	}

	if (!loader.linkProgram(shader.programID, shader.vertexID, shader.fragmentID)) {
		loader.unloadShader(shader);
		return error("ShaderLoader", "createProgramFromPaths", "Failed to link program");
	}

	shader.linked = true;

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
