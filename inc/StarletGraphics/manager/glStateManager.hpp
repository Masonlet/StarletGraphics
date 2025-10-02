#pragma once

class ShaderManager;

class GLStateManager {
public:
	GLStateManager(ShaderManager& sm);

	bool setProgram(const unsigned int id);
	unsigned int getProgram() const { return program; }

	bool init();
	void toggleWireframe();

private:
	unsigned int program{ 0 };
	bool wireframe{ false };
	ShaderManager& shaderManager;

	void setGLStateDefault();
};