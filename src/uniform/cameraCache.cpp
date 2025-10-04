#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletSerializer/utils/log.hpp"

bool CameraCache::cacheLocations() {
	return getUniformLocation(eyeLocation, "eyePos");
}