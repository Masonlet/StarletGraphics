#include "starlet-graphics/renderer/camera_renderer.hpp"
#include "starlet-graphics/uniform/uniform_cache.hpp"

#include "starlet-math/mat4.hpp"

#include <glad/glad.h>

namespace Starlet::Graphics {
	void CameraRenderer::updateCameraUniforms(const Math::Vec3<float>& eye, const Math::Mat4& view, const Math::Mat4& projection) const {
		glUniform3f(uniforms.getCameraCache().getEyeLocation(), eye.x, eye.y, eye.z);

		const ModelUL& modelUL = uniforms.getModelCache().getModelUL();
		glUniformMatrix4fv(modelUL.modelView, 1, GL_FALSE, view.ptr());
		glUniformMatrix4fv(modelUL.modelProj, 1, GL_FALSE, projection.ptr());
	}
}