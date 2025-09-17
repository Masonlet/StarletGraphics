#include "StarletGraphics/shader.hpp"
#include "StarletGraphics/shaderLoader.hpp"
#include "StarletParsers/fileParser.hpp"
#include "StarletParsers/utils/log.hpp"
#include <glad/glad.h>

void ShaderLoader::unloadShader(Shader& shader) {
	if (shader.programID && glIsProgram(shader.programID)) glDeleteProgram(shader.programID);
	if (shader.vertexID && glIsShader(shader.vertexID))   glDeleteShader(shader.vertexID);
	if (shader.fragmentID && glIsShader(shader.fragmentID)) glDeleteShader(shader.fragmentID);
	shader.programID = shader.vertexID = shader.fragmentID = 0;
	shader.linked = false;
}

bool ShaderLoader::loadAndCompileShader(unsigned int& outShaderID, int glShaderType, const std::string& path) {
	std::string source;
	if (!loadFile(source, path))
		return error("ShaderLoader", "createProgramFromPaths", "Failed to load vertex shader file");

	if (!compileShader(outShaderID, glShaderType, source))
		return error("ShaderLoader", "createProgramFromPaths", "Failed to compile vertex shader");

	return true;
}

bool ShaderLoader::compileShader(unsigned int& outShaderID, int glShaderType, const std::string& source) {
	outShaderID = glCreateShader(static_cast<GLenum>(glShaderType));
	if (!outShaderID) return error("ShaderLoader", "compileShader", "glCreateShader failed.");

	const char* src = source.c_str();
	glShaderSource(outShaderID, 1, &src, nullptr);
	glCompileShader(outShaderID);

	int status = GL_FALSE;
	glGetShaderiv(outShaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) return true;

	int logLen = 0;
	glGetShaderiv(outShaderID, GL_INFO_LOG_LENGTH, &logLen);
	std::string log;
	if (logLen > 1) {
		log.resize(static_cast<size_t>(logLen));
		int written = 0;
		glGetShaderInfoLog(outShaderID, logLen, &written, log.data());
		if (written > 0 && static_cast<size_t>(written) < log.size())
			log.resize(static_cast<size_t>(written));
	}

	glDeleteShader(outShaderID);
	outShaderID = 0;

	const std::string shaderType =
		(glShaderType == GL_VERTEX_SHADER ? "VERTEX"
			: glShaderType == GL_FRAGMENT_SHADER ? "FRAGMENT"
			: "UNKNOWN");
	return error("ShaderLoader", "compileShader", "Shader compilation failed (type=" + shaderType + "):\n" + log);
}

bool ShaderLoader::linkProgram(unsigned int& outProgramID, unsigned int vertID, unsigned int fragID) {
	outProgramID = glCreateProgram();
	if (!outProgramID) return error("ShaderLoader", "linkProgram", "glCreateProgram failed.");

	glAttachShader(outProgramID, vertID);
	glAttachShader(outProgramID, fragID);
	glLinkProgram(outProgramID);

	int status = GL_FALSE;
	glGetProgramiv(outProgramID, GL_LINK_STATUS, &status);

	glDetachShader(outProgramID, vertID);
	glDetachShader(outProgramID, fragID);
	if (status == GL_TRUE) return true;

	int logLen = 0;
	glGetProgramiv(outProgramID, GL_INFO_LOG_LENGTH, &logLen);

	std::string log;
	if (logLen > 1) {
		log.resize(static_cast<size_t>(logLen));
		int written = 0;
		glGetProgramInfoLog(outProgramID, logLen, &written, log.data());
		if (written > 0 && static_cast<size_t>(written) < log.size())
			log.resize(static_cast<size_t>(written));
	}

	glDeleteProgram(outProgramID);
	outProgramID = 0;
	return error("ShaderLoader", "linkProgram", std::string("Program link failed:\n") + log);
}