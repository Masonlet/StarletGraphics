#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletSerializer/utils/log.hpp"

namespace Starlet::Graphics {
	bool LightCache::cacheLocations() {
		bool ok = true;
		ok &= getUniformLocation(lightCountLocation, "lightCount");
		ok &= getUniformLocation(ambientLightLocation, "ambientLight");
		return ok;
	}
}