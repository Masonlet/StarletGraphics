#include "StarletGraphics/manager/shaderManager.hpp"
#include "StarletGraphics/resource/shaderCPU.hpp"

#include "StarletSerializer/parser.hpp"
#include "StarletSerializer/utils/log.hpp"

#include <glad/glad.h>
#include <sstream>

ShaderManager::~ShaderManager() {
	for (std::map<std::string, ShaderGPU>::iterator it = nameToShaders.begin(); it != nameToShaders.end(); ++it)
		handler.unload(it->second);
}

bool ShaderManager::useProgram(const std::string& name) const {
	std::map<std::string, ShaderGPU>::const_iterator it = nameToShaders.find(name);
	if (it == nameToShaders.end())
		return error("ShaderManager", "useProgram", "Shader not found: " + name);
	if (!it->second.linked || it->second.programID == 0)
		return error("ShaderManager", "useProgram", "Shader not linked, or has invalid id: " + name);

	glUseProgram(it->second.programID);
	return true;
}
bool ShaderManager::createProgramFromPaths(const std::string& name, const std::string& vertPath, const std::string& fragPath) {
	if (exists(name)) {
		std::map<std::string, ShaderGPU>::iterator it = nameToShaders.find(name);
		handler.unload(it->second);
		nameToShaders.erase(it);
	}

	Parser parser;
	ShaderCPU cpu;
	if (!parser.loadFile(cpu.vertexSource, basePath + vertPath))
		return error("ShaderLoader", "createProgramFromPaths", "Failed to load vertex shader source");

	if (!parser.loadFile(cpu.fragmentSource, basePath + fragPath))
		return error("ShaderLoader", "createProgramFromPaths", "Failed to load fragment shader source");

	cpu.vertexPath = vertPath;
	cpu.fragmentPath = fragPath;
	cpu.valid = true;

	ShaderGPU gpu;
	if (!handler.upload(cpu, gpu))
		return error("ShaderManager", "createProgramFromPaths", "Failed to upload shader");

	nameToShaders[name] = std::move(gpu);
	return true;
}


unsigned int ShaderManager::getProgramID(const std::string& name) const {
	std::map<std::string, ShaderGPU>::const_iterator it = nameToShaders.find(name);
	return (it == nameToShaders.end()) ? 0u : it->second.programID;
}

bool ShaderManager::getShader(const std::string& name, ShaderGPU*& dataOut) {
	std::map<std::string, ShaderGPU>::iterator it = nameToShaders.find(name);
	if (it == nameToShaders.end()) return error("ShaderManager", "getShader", "Shader not found: " + name);
	dataOut = &it->second;
	return true;
}
bool ShaderManager::getShader(const std::string& name, const ShaderGPU*& dataOut) const {
	std::map<std::string, ShaderGPU>::const_iterator it = nameToShaders.find(name);
	if (it == nameToShaders.end()) return error("ShaderManager", "getShader", "Shader not found: " + name);
	dataOut = &it->second;
	return true;
}
