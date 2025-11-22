#include "starlet-graphics/uniform/uniform_cache.hpp"

namespace Starlet::Graphics {
	bool LightCache::cacheLocations() {
		bool ok = true;
		ok &= getUniformLocation(lightCountLocation, "lightCount");
		ok &= getUniformLocation(ambientLightLocation, "ambientLight");
		return ok;
	}
}