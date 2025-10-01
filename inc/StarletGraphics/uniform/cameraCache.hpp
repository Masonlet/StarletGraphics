#pragma once

#include "StarletGraphics/uniform/cache.hpp"

class CameraCache : public Cache {
public:
	bool cacheLocations() override;
	int getEyeLocation() const { return eyeLocation; }

private:
	int eyeLocation{ -1 };
};