#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletParser/utils/log.hpp"

#include <glad/glad.h>

bool Cache::getUniformLocation(int& location, const char* name) const {
	location = glGetUniformLocation(program, name);
	if (location < 0) return error("UniformCache", "getUniformLocation", std::string("Could not find uniform: ") + name);
	return true;
}