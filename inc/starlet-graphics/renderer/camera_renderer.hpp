#pragma once

namespace Starlet {
	namespace Math {
		template <typename T> struct Vec3;
		struct Mat4;
	}

	namespace Graphics {
		class UniformCache;

		class CameraRenderer {
		public:
			CameraRenderer(const UniformCache& uc) : uniforms(uc) {}

			void updateCameraUniforms(const Math::Vec3<float>& eye, const Math::Mat4& view, const Math::Mat4& projection) const;
		private:
			const UniformCache& uniforms;
		};
	}
}