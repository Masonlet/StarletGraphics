#include "StarletGraphics/renderer/lightRenderer.hpp"
#include "StarletGraphics/uniform/uniformCache.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletScene/scene.hpp"
#include "StarletScene/components/light.hpp"
#include "StarletScene/components/transform.hpp"
#include "StarletScene/components/colour.hpp"

#include <glad/glad.h>

void LightRenderer::updateLightUniforms(const unsigned int program, const Scene& scene) const {
	auto lightEntities = scene.getEntitiesOfType<Light>();
	updateLightCount(static_cast<int>(lightEntities.size()));

	const unsigned int location = uniforms.getLightCache().getAmbientLightLocation();
	if (location != -1) glUniform4fv(location, 1, &scene.getAmbientLight().x);

	int lightIndex = 0;
	for (const auto& [entity, light] : lightEntities) {
		std::string prefix = "theLights[" + std::to_string(lightIndex) + "].";
		int pos_UL = glGetUniformLocation(program, (prefix + "position").c_str());
		int diff_UL = glGetUniformLocation(program, (prefix + "diffuse").c_str());
		int atten_UL = glGetUniformLocation(program, (prefix + "attenuation").c_str());
		int dir_UL = glGetUniformLocation(program, (prefix + "direction").c_str());
		int param1_UL = glGetUniformLocation(program, (prefix + "param1").c_str());
		int param2_UL = glGetUniformLocation(program, (prefix + "param2").c_str());

		if (!light->enabled || !scene.hasComponent<TransformComponent>(entity)) {
			if (param2_UL != -1) glUniform4f(param2_UL, 0.0f, 0.0f, 0.0f, 0.0f);
			++lightIndex;
			continue;
		}

		const TransformComponent& transform = scene.getComponent<TransformComponent>(entity);
		const ColourComponent& colour = scene.getComponent<ColourComponent>(entity);
		if (pos_UL != -1)    glUniform4f(pos_UL, transform.pos.x, transform.pos.y, transform.pos.z, 1.0f);
		if (diff_UL != -1)   glUniform4fv(diff_UL, 1, &colour.colour.r);
		if (atten_UL != -1)  glUniform4fv(atten_UL, 1, &light->attenuation.r);
		if (dir_UL != -1)    glUniform4f(dir_UL, transform.rot.r, transform.rot.g, transform.rot.b, 1.0f);
		if (param1_UL != -1) glUniform4f(param1_UL, static_cast<float>(light->type), light->param1.x, light->param1.y, 0.0f);
		if (param2_UL != -1) glUniform4f(param2_UL, light->enabled ? 1.0f : 0.0f, 0.0f, 0.0f, 0.0f);
		++lightIndex;
	}
}
void LightRenderer::updateLightCount(int count) const {
	const unsigned int location = uniforms.getLightCache().getLightCountLocation();
	if (location != -1) glUniform1i(location, count);
}