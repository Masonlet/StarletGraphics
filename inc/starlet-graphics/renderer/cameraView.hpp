#pragma once

#include "starlet-math/vec3.hpp"

namespace Starlet::Graphics {
	struct CameraView {
		Math::Vec3<float> eye;
		Math::Vec3<float> front;
		Math::Vec3<float> right;
		Math::Vec3<float> up;

		static CameraView fromTransform(const Math::Vec3<float>& pos, const Math::Vec3<float>& rot, const Math::Vec3<float>& up);
	};
}