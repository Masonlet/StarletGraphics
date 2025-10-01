#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletParser/utils/log.hpp"

bool CameraCache::cacheLocations() {
	return getUniformLocation(eyeLocation, "eyePos");
}