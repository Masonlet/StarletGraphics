#pragma once

#include "starlet-graphics/uniform/model_cache.hpp"
#include "starlet-graphics/uniform/light_cache.hpp"
#include "starlet-graphics/uniform/camera_cache.hpp"

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