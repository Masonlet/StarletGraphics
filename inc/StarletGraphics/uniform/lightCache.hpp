#pragma once

#include "StarletGraphics/uniform/cache.hpp"

namespace Starlet::Graphics {
	struct LightUL {
		int position_UL{ -1 };
		int diffuse_UL{ -1 };
		int attenuation_UL{ -1 };
		int direction_UL{ -1 };
		int param1_UL{ -1 };
		int param2_UL{ -1 };
	};

	class LightCache : public Cache {
	public:
		bool cacheLocations() override;
		int getLightCountLocation() const { return lightCountLocation; }
		int getAmbientLightLocation() const { return ambientLightLocation; }
		const LightUL& getLightUL() const { return uniform; }

	private:
		int lightCountLocation{ -1 };
		int ambientLightLocation{ -1 };
		LightUL uniform;
	};
}