#include "StarletGraphics/renderer/cameraRenderer.hpp"
#include "StarletGraphics/uniform/uniformCache.hpp"

#include "StarletMath/mat4.hpp"

#include <glad/glad.h>

void CameraRenderer::updateCameraUniforms(const Vec3<float>& eye, const Mat4& view, const Mat4& projection) const {
	glUniform3f(uniforms.getCameraCache().getEyeLocation(), eye.x, eye.y, eye.z);

	const ModelUL& modelUL = uniforms.getModelCache().getModelUL();
	glUniformMatrix4fv(modelUL.modelView, 1, GL_FALSE, view.ptr());
	glUniformMatrix4fv(modelUL.modelProj, 1, GL_FALSE, projection.ptr());
}