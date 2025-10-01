#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletParser/utils/log.hpp"

bool UniformCache::setProgram(unsigned int programID) {
	if (programID == 0) return error("UniformCache", "setProgram", "Program ID is 0");

	program = programID;
	modelCache.setProgram(programID);
	lightCache.setProgram(programID);
	cameraCache.setProgram(programID);
	return true;
}
bool UniformCache::cacheAllLocations() {
	if (program == 0) return error("UniformCache", "cacheAllLocations", "Program ID is 0");

	bool ok = true;
	ok &= cameraCache.cacheLocations();
	ok &= modelCache.cacheLocations();
	ok &= lightCache.cacheLocations();
	return ok;
}