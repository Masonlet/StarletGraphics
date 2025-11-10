#pragma once

#include "starlet-graphics/uniform/cache.hpp"

namespace Starlet::Graphics {
	class CameraCache : public Cache {
	public:
		bool cacheLocations() override;
		int getEyeLocation() const { return eyeLocation; }

	private:
		int eyeLocation{ -1 };
	};
}