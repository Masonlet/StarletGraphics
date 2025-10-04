#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletSerializer/utils/log.hpp"

bool LightCache::cacheLocations() {
	bool ok = true;
	ok &= getUniformLocation(lightCountLocation, "lightCount");
	ok &= getUniformLocation(ambientLightLocation, "ambientLight");
	return ok;
}