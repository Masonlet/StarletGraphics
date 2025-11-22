#pragma once

namespace Starlet::Graphics {
	class GLStateManager {
	public:
		GLStateManager() = default;

		bool setProgram(const unsigned int id);
		unsigned int getProgram() const { return program; }

		void toggleWireframe();
		void setGLStateDefault();

	private:
		unsigned int program{ 0 };
		bool wireframe{ false };
	};
}