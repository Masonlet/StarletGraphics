#include "starlet-graphics/uniform/uniform_cache.hpp"

namespace Starlet::Graphics {
	bool CameraCache::cacheLocations() {
		return getUniformLocation(eyeLocation, "eyePos");
	}
}