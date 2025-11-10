#include "starlet-graphics/manager/glStateManager.hpp"
#include "starlet-logger/logger.hpp"

#include <glad/glad.h>

namespace Starlet::Graphics {
	bool GLStateManager::setProgram(const unsigned int id) {
		if (id == 0) return Logger::error("Renderer", "setProgram", "Program is 0");

		glUseProgram(id);
		this->program = id;
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
}
