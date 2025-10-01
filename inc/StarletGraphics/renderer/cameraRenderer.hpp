#pragma once

class UniformCache;
template <typename T> struct Vec3;
struct Mat4;

class CameraRenderer {
public:
  CameraRenderer(const UniformCache& uc) : uniforms(uc) {}

	void updateCameraUniforms(const Vec3<float>& eye, const Mat4& view, const Mat4& projection) const;
private:
	const UniformCache& uniforms;
};