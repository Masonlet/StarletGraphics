#pragma once

#include "StarletGraphics/uniform/modelCache.hpp"
#include "StarletGraphics/uniform/lightCache.hpp"
#include "StarletGraphics/uniform/cameraCache.hpp"

namespace Starlet::Graphics {
	class UniformCache {
	public:
		bool setProgram(unsigned int programID);
		bool cacheAllLocations();

		const ModelCache& getModelCache() const { return modelCache; }
		const LightCache& getLightCache() const { return lightCache; }
		const CameraCache& getCameraCache() const { return cameraCache; }

	private:
		unsigned int program{ 0 };
		ModelCache modelCache;
		LightCache lightCache;
		CameraCache cameraCache;
	};
}