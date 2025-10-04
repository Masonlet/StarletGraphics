#include "StarletGraphics/manager/shaderManager.hpp"
#include "StarletGraphics/manager/glStateManager.hpp"
#include "StarletSerializer/utils/log.hpp"
#include <glad/glad.h>

bool GLStateManager::init() {
	if (!shaderManager.createProgramFromPaths("shader1", "vertex_shader.glsl", "fragment_shader.glsl"))
		return error("Engine", "initialize", "Failed to create shader program from file");

	if (!setProgram(shaderManager.getProgramID("shader1")))
		return error("GLStateManager", "init", "Failed to set initial program");

	setGLStateDefault();
	return true;
}

bool GLStateManager::setProgram(const unsigned int id) {
	if (id == 0) return error("Renderer", "setProgram", "Program is 0");
	this->program = id;
	glUseProgram(id);
	return true;
}

void GLStateManager::setGLStateDefault() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void GLStateManager::toggleWireframe() {
	wireframe = !wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}
