#include "StarletGraphics/handler/shaderHandler.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletGraphics/resource/shaderGPU.hpp"
#include "StarletGraphics/resource/shaderCPU.hpp"

#include <glad/glad.h>

namespace Starlet::Graphics {
	bool ShaderHandler::upload(ShaderCPU& cpu, ShaderGPU& gpu) {
		if (cpu.empty()) return Serializer::error("ShaderHandler", "upload", "Attempting to upload empty Shader");

		if (!compileShader(gpu.vertexID, GL_VERTEX_SHADER, cpu.vertexSource)) return false;
		if (!compileShader(gpu.fragmentID, GL_FRAGMENT_SHADER, cpu.fragmentSource)) {
			glDeleteShader(gpu.vertexID);
			return false;
		}

		if (!linkProgram(gpu.programID, gpu.vertexID, gpu.fragmentID)) {
			glDeleteShader(gpu.vertexID);
			glDeleteShader(gpu.fragmentID);
			return false;
		}

		gpu.linked = true;
		return true;
	}

	bool ShaderHandler::compileShader(unsigned int& outShaderID, int glShaderType, const std::string& source) {
		outShaderID = glCreateShader(static_cast<GLenum>(glShaderType));
		if (!outShaderID) return Serializer::error("ShaderHandler", "compileShader", "glCreateShader failed");

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
		return Serializer::error("ShaderHandler", "compileShader", "Shader compilation failed (type=" + shaderType + "):\n" + log);
	}

	bool ShaderHandler::linkProgram(unsigned int& outProgramID, unsigned int vertID, unsigned int fragID) {
		outProgramID = glCreateProgram();
		if (!outProgramID) return Serializer::error("ShaderHandler", "linkProgram", "glCreateProgram failed");

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
		return Serializer::error("ShaderHandler", "linkProgram", std::string("Program link failed:\n") + log);
	}

	void ShaderHandler::unload(ShaderGPU& shader) {
		if (shader.programID && glIsProgram(shader.programID))  glDeleteProgram(shader.programID);
		if (shader.vertexID && glIsShader(shader.vertexID))     glDeleteShader(shader.vertexID);
		if (shader.fragmentID && glIsShader(shader.fragmentID)) glDeleteShader(shader.fragmentID);
		shader.programID = shader.vertexID = shader.fragmentID = 0;
		shader.linked = false;
	}
}