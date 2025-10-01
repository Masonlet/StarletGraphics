#pragma once

#include "StarletMath/vec3.hpp"

struct CameraView {
	Vec3<float> eye;
	Vec3<float> front;
	Vec3<float> right;
	Vec3<float> up;

	static CameraView fromTransform(const Vec3<float>& pos, const Vec3<float>& rot, const Vec3<float>& up);
};