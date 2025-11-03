#include "StarletGraphics/uniform/uniformCache.hpp"

namespace Starlet::Graphics {
	bool CameraCache::cacheLocations() {
		return getUniformLocation(eyeLocation, "eyePos");
	}
}