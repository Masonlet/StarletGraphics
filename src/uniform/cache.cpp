#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletLogger/logger.hpp"

#include <glad/glad.h>

namespace Starlet::Graphics {
	bool Cache::getUniformLocation(int& location, const char* name) const {
		location = glGetUniformLocation(program, name);
		if (location < 0) return Logger::error("UniformCache", "getUniformLocation", std::string("Could not find uniform: ") + name);
		return true;
	}
}