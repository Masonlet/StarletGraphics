#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletSerializer/utils/log.hpp"

namespace Starlet::Graphics {
	bool CameraCache::cacheLocations() {
		return getUniformLocation(eyeLocation, "eyePos");
	}
}