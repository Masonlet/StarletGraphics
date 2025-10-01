#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletParser/utils/log.hpp"

bool LightCache::cacheLocations() {
	bool ok = true;
	ok &= getUniformLocation(lightCountLocation, "lightCount");
	ok &= getUniformLocation(ambientLightLocation, "ambientLight");
	ok &= getUniformLocation(uniform.position_UL, "theLights[0].position");
	ok &= getUniformLocation(uniform.diffuse_UL, "theLights[0].diffuse");
	ok &= getUniformLocation(uniform.attenuation_UL, "theLights[0].attenuation");
	ok &= getUniformLocation(uniform.direction_UL, "theLights[0].direction");
	ok &= getUniformLocation(uniform.param1_UL, "theLights[0].param1");
	ok &= getUniformLocation(uniform.param2_UL, "theLights[0].param2");
	return ok;
}