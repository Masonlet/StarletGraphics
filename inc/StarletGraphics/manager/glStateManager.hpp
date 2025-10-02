#pragma once

class ShaderManager;

class GLStateManager {
public:
	GLStateManager() = default;

	bool setProgram(const unsigned int id);
	unsigned int getProgram() const { return program; }

	bool init();
	void toggleWireframe();
	void setBasePath(const std::string& path) { shaderManager.setBasePath(path); }

private:
	unsigned int program{ 0 };
	bool wireframe{ false };
	ShaderManager shaderManager;

	void setGLStateDefault();
};